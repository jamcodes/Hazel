#include "WindowsInput.h"

#include <GLFW/glfw3.h>

#include "WindowsWindow.h"

#include "Hazel/Core/Application.h"

namespace Hazel {

Input* Input::s_instance_{new WindowsInput{}};  // Leak Input instance

GLFWwindow* WindowsInput::getWindowHandle() noexcept
{
    return static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
}

bool WindowsInput::isKeyPressedImpl(int keycode) const
{
    auto* window{getWindowHandle()};
    auto const state{glfwGetKey(window, keycode)};
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::isMouseButtonPressedImpl(int button) const
{
    auto* window{getWindowHandle()};
    auto const state{glfwGetMouseButton(window, button)};
    return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::getMousePositionImpl() const
{
    auto* window{getWindowHandle()};
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {static_cast<float>(xpos), static_cast<float>(ypos)};
}

float WindowsInput::getMouseXImpl() const
{
    auto [x, y]{getMousePositionImpl()};
    return x;
}

float WindowsInput::getMouseYImpl() const
{
    auto [x, y]{getMousePositionImpl()};
    return y;
}


}  // namespace Hazel
