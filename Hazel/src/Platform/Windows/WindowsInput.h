#pragma once

#include "Hazel/Input.h"
#include <GLFW/glfw3.h>

namespace Hazel {
class WindowsInput : public Input {
public:
    WindowsInput() = default;
    ~WindowsInput() override = default;
    WindowsInput& operator=(WindowsInput&&) = delete;

private:
    inline static GLFWwindow* getWindowHandle() noexcept;

    bool isKeyPressedImpl(int keycode) const override;
    bool isMouseButtonPressedImpl(int button) const override;
    float getMouseXImpl() const override;
    float getMouseYImpl() const override;
    std::pair<float, float> getMousePositionImpl() const override;
};
}  // namespace Hazel
