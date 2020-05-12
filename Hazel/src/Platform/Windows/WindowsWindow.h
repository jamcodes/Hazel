#pragma once

#include "Hazel/Core/Window.h"

#include "Hazel/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

#include <type_traits>

namespace Hazel {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    ~WindowsWindow() override = default;

    void onUpdate() noexcept override;

    inline unsigned int getWidth() const noexcept override { return data_.width; }
    inline unsigned int getHeight() const noexcept override { return data_.height; }

    // Window attributes
    inline void setEventCallback(EventCallbackFn callback) noexcept(
        std::is_nothrow_move_constructible_v<EventCallbackFn>) override
    {
        data_.eventCallback = std::move(callback);
    }
    void setVSync(bool enabled) noexcept override;
    bool isVSync() const noexcept override;

    inline void* getNativeWindow() const noexcept override;

private:
    void setGlfwCallbacks() noexcept;

    static void shutdown(GLFWwindow*) noexcept;

    struct GLFWwindowDestructor final {
        inline void operator()(GLFWwindow* p) const noexcept
        {
            shutdown(p);
        }
    };

    struct WindowData {
        std::string title;
        unsigned width, height;
        bool vSync;
        EventCallbackFn eventCallback;
    };

    static WindowData& getWindowData(GLFWwindow* p) noexcept
    {
        return *static_cast<WindowData*>(glfwGetWindowUserPointer(p));
    }

    std::unique_ptr<GLFWwindow, GLFWwindowDestructor> window_{nullptr};
    WindowData data_;
    std::unique_ptr<GraphicsContext> context_{nullptr};
};

}  // namespace Hazel
