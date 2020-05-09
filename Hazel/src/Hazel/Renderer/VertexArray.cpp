#include "VertexArray.h"

#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

Scope<VertexArray> VertexArray::create()
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexArray>();
    default:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

}  // namespace Hazel
