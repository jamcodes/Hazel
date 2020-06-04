#include "OpenGLVertexArray.h"

#include <glad/glad.h>

#include "Hazel/Core/AssertionHandler.h"

namespace Hazel {
template <>
constexpr GLenum shaderDataTypeToPlatformType<GLenum>(ShaderDataType type) noexcept
{
    // clang-format off
    switch(type) {
        case ShaderDataType::None  :    return GL_NONE;
        case ShaderDataType::Float :    return GL_FLOAT;
        case ShaderDataType::Float2:    return GL_FLOAT;
        case ShaderDataType::Float3:    return GL_FLOAT;
        case ShaderDataType::Float4:    return GL_FLOAT;
        case ShaderDataType::Mat3  :    return GL_FLOAT;
        case ShaderDataType::Mat4  :    return GL_FLOAT;
        case ShaderDataType::Int   :    return GL_INT;
        case ShaderDataType::Int2  :    return GL_INT;
        case ShaderDataType::Int3  :    return GL_INT;
        case ShaderDataType::Int4  :    return GL_INT;
        case ShaderDataType::Bool  :    return GL_BOOL;
    }
    // clang-format on
    HZ_EXPECTS(false, DefaultCoreHandler, Enforce, "Unknown ShaderDataType");
    return GL_NONE;
}

OpenGLVertexArray::OpenGLVertexArray() noexcept
{
    HZ_PROFILE_FUNCTION();
    glCreateVertexArrays(1, &renderer_id_);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    HZ_PROFILE_FUNCTION();
    glDeleteVertexArrays(1, &renderer_id_);
}

void OpenGLVertexArray::bind() const
{
    HZ_PROFILE_FUNCTION();
    glBindVertexArray(renderer_id_);
}

void OpenGLVertexArray::unbind() const
{
    HZ_PROFILE_FUNCTION();
    glBindVertexArray(0);
}

inline void OpenGLVertexArray::defineVertexAttributeArray(BufferElement const& element, std::uint32_t stride) noexcept
{
    const auto count{componentCount(element.type)};
    const auto native_type{shaderDataTypeToPlatformType<GLenum>(element.type)};
    const auto is_normalized{element.normalized ? GL_TRUE : GL_FALSE};
    switch (element.type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
    case ShaderDataType::Bool: {
        glVertexAttribPointer(vertex_buffer_index_, count, native_type, is_normalized, stride,
                              reinterpret_cast<const void*>(element.offset));
        break;
    }
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4: {
        for (std::uint32_t i{0}; i != count; ++i) {
            glVertexAttribPointer(vertex_buffer_index_, count, native_type, is_normalized, stride,
                                  reinterpret_cast<const void*>(sizeof(float) * count * i));
            glVertexAttribDivisor(vertex_buffer_index_, 1);
        }
        break;
    }
    default:
        HZ_ASSERT(false, "Unknown ShaderDataType!");
    }
}

void OpenGLVertexArray::addVertexBuffer(Scope<VertexBuffer> p_vertex_buffer)
{
    HZ_PROFILE_FUNCTION();
    HZ_EXPECTS(p_vertex_buffer != nullptr, DefaultCoreHandler, Enforce, "VertexBuffer* may not be nullptr");
    HZ_EXPECTS(!p_vertex_buffer->getLayout().getElements().empty(), DefaultCoreHandler, Enforce,
               "VertexBuffer must have a layout set");

    glBindVertexArray(renderer_id_);
    p_vertex_buffer->bind();

    auto const& vb_layout{p_vertex_buffer->getLayout()};
    for (const auto& element : vb_layout) {
        glEnableVertexAttribArray(vertex_buffer_index_);
        defineVertexAttributeArray(element, vb_layout.getStride());
        ++vertex_buffer_index_;
    }

    vertex_buffers_.push_back(std::move(p_vertex_buffer));
}

void OpenGLVertexArray::setIndexBuffer(Scope<IndexBuffer> p_index_buffer)
{
    HZ_PROFILE_FUNCTION();
    glBindVertexArray(renderer_id_);
    p_index_buffer->bind();
    index_buffer_ = std::move(p_index_buffer);
}

}  // namespace Hazel
