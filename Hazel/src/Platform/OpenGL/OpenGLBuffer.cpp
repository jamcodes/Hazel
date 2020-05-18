#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel {
// --- OpenGLIndexBuffer ---
// ------------------------------------------------------------------------------------------------
OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const std::uint32_t size)
{
    HZ_PROFILE_FUNCTION();
    glCreateBuffers(1, &renderer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(*vertices), vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    HZ_PROFILE_FUNCTION();
    glDeleteBuffers(1, &renderer_id_);
}

void OpenGLVertexBuffer::bind() const noexcept
{
    HZ_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

void OpenGLVertexBuffer::unbind() const noexcept
{
    HZ_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// ------------------------------------------------------------------------------------------------

// --- OpenGLIndexBuffer ---
// ------------------------------------------------------------------------------------------------
OpenGLIndexBuffer::OpenGLIndexBuffer(const std::uint32_t* indices, const std::uint32_t size) : count_{size}
{
    HZ_PROFILE_FUNCTION();
    glCreateBuffers(1, &renderer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(*indices), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    HZ_PROFILE_FUNCTION();
    glDeleteBuffers(1, &renderer_id_);
}

void OpenGLIndexBuffer::bind() const noexcept
{
    HZ_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
}

void OpenGLIndexBuffer::unbind() const noexcept
{
    HZ_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
// ------------------------------------------------------------------------------------------------

}  // namespace Hazel
