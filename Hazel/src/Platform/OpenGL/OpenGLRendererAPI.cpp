#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Hazel {

void OpenGLRendererAPI::init()
{
    HZ_PROFILE_FUNCTION();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::setViewport(unsigned x, unsigned y, unsigned width, unsigned height)
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::setClearColor(glm::vec4 const& color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::drawIndexed(VertexArray const& vertex_array)
{
    glDrawElements(GL_TRIANGLES, vertex_array.getIndexBuffer().getCount(), GL_UNSIGNED_INT,
                   nullptr);
}

}  // namespace Hazel
