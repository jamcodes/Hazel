#include "Framebuffer.h"

#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hazel {

Ref<Framebuffer> Framebuffer::create(FramebufferSpecification const& spec)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_ASSERT(false, "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return makeRef<OpenGLFramebuffer>(spec);
    default:
        HZ_ASSERT(false, "Unknown RendererAPI::API");
    }

    return nullptr;
}

}  // namespace Hazel
