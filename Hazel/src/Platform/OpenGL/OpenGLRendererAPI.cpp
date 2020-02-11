#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Hazel {

void OpenGLRendererAPI::setClearColor(glm::vec4 const& color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::drawIndexed(VertexArray const& vertex_array)
{
    glDrawElements(GL_TRIANGLES, vertex_array.getIndexBuffer().getCount(), GL_UNSIGNED_INT,
                   nullptr);
}

}  // namespace Hazel
