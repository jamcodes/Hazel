#include "hzpch.h"

#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Core/AssertionHandler.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel {

struct WindowsWindowAssertionHandler : CoreLoggingHandler, Hazel::Enforce {
};

static bool s_GLFWInitialized = false;
static int s_glfw_window_count{0};

static void GLFWErrorCallback(int error, const char* description) noexcept
{
    HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

std::unique_ptr<Window> Window::create(const WindowProps& props)
{
    return std::make_unique<WindowsWindow>(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
    : data_{props.title, props.width, props.height}
{
    HZ_PROFILE_FUNCTION();
    HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

    // if (!s_GLFWInitialized) {
    if (s_glfw_window_count == 0) {
        // TODO: glfwTerminate on system shutdown
        HZ_CORE_INFO("Initializing GLFW");
        const int rc = glfwInit();
        HZ_ASSERT(rc, WindowsWindowAssertionHandler, Hazel::Enforce, "Could not intialize GLFW!");

        glfwSetErrorCallback(GLFWErrorCallback);
        // s_GLFWInitialized = true;
    }
    window_.reset(glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height),
                                   props.title.c_str(), nullptr, nullptr));
    ++s_glfw_window_count;
    context_ = std::make_unique<OpenGLContext>(window_.get());

    glfwSetWindowUserPointer(window_.get(), &data_);
    setVSync(true);
    setGlfwCallbacks();
}

void WindowsWindow::shutdown(GLFWwindow* p) noexcept
{
    HZ_PROFILE_FUNCTION();
    glfwDestroyWindow(p);
    if (--s_glfw_window_count == 0) {
        HZ_CORE_INFO("Terminating GLFW");
        glfwTerminate();
    }
}

void WindowsWindow::setGlfwCallbacks() noexcept
{
    glfwSetWindowSizeCallback(window_.get(), [](GLFWwindow* window, int width, int height) {
        WindowData& data = getWindowData(window);
        data.width = width;
        data.height = height;

        WindowResizeEvent e{static_cast<unsigned>(width), static_cast<unsigned>(height)};
        data.eventCallback(e);
    });

    glfwSetWindowCloseCallback(window_.get(), [](GLFWwindow* window) {
        auto& data = getWindowData(window);
        WindowCloseEvent e;
        data.eventCallback(e);
    });

    glfwSetKeyCallback(window_.get(),
                       [](GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */) {
                           auto& data = getWindowData(window);
                           switch (action) {
                           case GLFW_PRESS: {
                               KeyPressedEvent e{key, 0};
                               data.eventCallback(e);
                               break;
                           }
                           case GLFW_RELEASE: {
                               KeyReleasedEvent e{key};
                               data.eventCallback(e);
                               break;
                           }
                           case GLFW_REPEAT: {
                               KeyPressedEvent e{key, 1};
                               data.eventCallback(e);
                               break;
                           }
                           }
                       });

    glfwSetCharCallback(window_.get(), [](GLFWwindow* window, unsigned int keycode) {
        auto& data = getWindowData(window);
        KeyTypedEvent e{static_cast<int>(keycode)};
        data.eventCallback(e);
    });

    glfwSetMouseButtonCallback(window_.get(),
                               [](GLFWwindow* window, int button, int action, int /* mods */) {
                                   auto& data = getWindowData(window);

                                   switch (action) {
                                   case GLFW_PRESS: {
                                       MouseButtonPressedEvent e{button};
                                       data.eventCallback(e);
                                       break;
                                   }
                                   case GLFW_RELEASE: {
                                       MouseButtonReleasedEvent e{button};
                                       data.eventCallback(e);
                                       break;
                                   }
                                   }
                               });

    glfwSetScrollCallback(window_.get(), [](GLFWwindow* window, double xOffset, double yOffset) {
        auto& data = getWindowData(window);
        MouseScrolledEvent e{static_cast<float>(xOffset), static_cast<float>(yOffset)};
        data.eventCallback(e);
    });

    glfwSetCursorPosCallback(window_.get(), [](GLFWwindow* window, double xPos, double yPos) {
        auto& data = getWindowData(window);
        MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
        data.eventCallback(event);
    });
}

void WindowsWindow::onUpdate() noexcept
{
    HZ_PROFILE_FUNCTION();
    glfwPollEvents();
    context_->swapBuffers();
}

void WindowsWindow::setVSync(bool enabled) noexcept
{
    HZ_PROFILE_FUNCTION();
    glfwSwapInterval(static_cast<int>(enabled));
    data_.vSync = enabled;
}

bool WindowsWindow::isVSync() const noexcept { return data_.vSync; }

void* WindowsWindow::getNativeWindow() const noexcept
{
    return window_.get();
}


}  // namespace Hazel
