#include "VertexArray.h"

#include "Hazel/AssertionHandler.h"
#include "Hazel/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

std::unique_ptr<VertexArray> VertexArray::create()
{
    switch (Renderer::getApi()) {
    case RendererAPI::None:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::None is currently not supported");
    case RendererAPI::OpenGL:
        return std::make_unique<OpenGLVertexArray>();
    default:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI");
    }

    return nullptr;
}

}  // namespace Hazel
