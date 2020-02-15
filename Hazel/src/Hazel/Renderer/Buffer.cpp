#include "Buffer.h"

#include "Renderer.h"

#include "Hazel/AssertionHandler.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

std::unique_ptr<VertexBuffer> VertexBuffer::create(const float* vertices, std::uint32_t size)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexBuffer>(vertices, size);
    default:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

std::unique_ptr<IndexBuffer> IndexBuffer::create(const std::uint32_t* indices, std::uint32_t size)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLIndexBuffer>(indices, size);
    default:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

}  // namespace Hazel