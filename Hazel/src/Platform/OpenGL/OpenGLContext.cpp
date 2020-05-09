#include "OpenGLContext.h"

#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"

#include <glad/glad.h>

namespace {

struct OpenGLContextAssertHandler : Hazel::CoreLoggingHandler, Hazel::Enforce {
};
}  // namespace

namespace Hazel {
OpenGLContext::OpenGLContext(GLFWwindow* window_handle) : window_handle_{window_handle}
{
    HZ_EXPECTS(window_handle != nullptr, ::OpenGLContextAssertHandler, Hazel::Enforce,
               "Window handle can not be nullptr");
    glfwMakeContextCurrent(window_handle_);
    initGLLoader();
    glInfo();
}

void OpenGLContext::swapBuffers() noexcept { glfwSwapBuffers(window_handle_); }

void OpenGLContext::initGLLoader() noexcept
{
    auto const rc{gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))};
    HZ_ASSERT(rc, ::OpenGLContextAssertHandler, Hazel::Enforce, "Could not initialize glad!");
}

void OpenGLContext::glInfo() noexcept
{
    HZ_CORE_INFO("OpenGL Info");
    HZ_CORE_INFO("    Vendor: {}", glGetString(GL_VENDOR));
    HZ_CORE_INFO("    Renderer: {}", glGetString(GL_RENDERER));
    HZ_CORE_INFO("    Version: {}", glGetString(GL_VERSION));
}

}  // namespace Hazel
