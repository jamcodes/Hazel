#pragma once

#include <GLFW/glfw3.h>

#include "Hazel/Renderer/GraphicsContext.h"

namespace Hazel {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(GLFWwindow* window_handle);
    ~OpenGLContext() noexcept override = default;
    OpenGLContext& operator=(OpenGLContext&&) noexcept = delete;

    void swapBuffers() noexcept override;
private:
    inline void initGLLoader() noexcept;

    GLFWwindow* window_handle_;
};

}  // namespace Hazel
