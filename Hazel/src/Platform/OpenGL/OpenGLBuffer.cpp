#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel
{
// --- OpenGLIndexBuffer ---
// ------------------------------------------------------------------------------------------------
OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const std::uint32_t size)
{
    glCreateBuffers(1, &renderer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(*vertices), vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &renderer_id_);
}

void OpenGLVertexBuffer::bind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

void OpenGLVertexBuffer::unbind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// ------------------------------------------------------------------------------------------------

// --- OpenGLIndexBuffer ---
// ------------------------------------------------------------------------------------------------
OpenGLIndexBuffer::OpenGLIndexBuffer(const std::uint32_t* indices, const std::uint32_t size)
    : count_{size}
{
    glCreateBuffers(1, &renderer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(*indices), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &renderer_id_);
}

void OpenGLIndexBuffer::bind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
}

void OpenGLIndexBuffer::unbind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
// ------------------------------------------------------------------------------------------------

} // namespace Hazel
