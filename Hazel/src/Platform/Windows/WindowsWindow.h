#pragma once

#include <type_traits>

#include "Hazel/Window.h"

#include <GLFW/glfw3.h>

namespace Hazel {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow() = default;

    void onUpdate() override;

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

private:
    inline void initGlLoader() noexcept;
    void setGlfwCallbacks() noexcept;

    struct GLFWwindowDestructor {
        inline void operator()(GLFWwindow* p) noexcept { glfwDestroyWindow(p); }
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
};

}  // namespace Hazel
