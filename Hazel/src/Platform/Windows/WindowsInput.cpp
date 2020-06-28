
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Application.h"

#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

namespace
{

GLFWwindow* getWindowHandle() noexcept
{
    return static_cast<GLFWwindow*>(Hazel::Application::get().getWindow().getNativeWindow());
}

} // namespace

namespace Hazel {

bool Input::isKeyPressed(int keycode) noexcept
{
    auto* window{getWindowHandle()};
    auto const state{glfwGetKey(window, keycode)};
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isKeyPressed(KeyCode keycode) noexcept
{
    return isKeyPressed(toInteger<int>(keycode));
}

bool Input::isMouseButtonPressed(int button) noexcept
{
    auto* window{getWindowHandle()};
    auto const state{glfwGetMouseButton(window, button)};
    return state == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(MouseButton button) noexcept
{
    return isMouseButtonPressed(toInteger<int>(button));
}

std::pair<float, float> Input::getMousePosition() noexcept
{
    auto* window{getWindowHandle()};
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {static_cast<float>(xpos), static_cast<float>(ypos)};
}

float Input::getMouseX() noexcept
{
    auto const [x, y]{getMousePosition()};
    return x;
}

float Input::getMouseY() noexcept
{
    auto const [x, y]{getMousePosition()};
    return y;
}

}  // namespace Hazel
