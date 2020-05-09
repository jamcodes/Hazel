#include "Texture.h"

#include "Hazel/Core/AssertionHandler.h"

#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {

Ref<Texture2D> Texture2D::create(const std::string& path)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLTexture2D>(path);
    default:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

template <>
Ref<OpenGLTexture2D> Texture2D::create<OpenGLTexture2D>(const std::string& path)
{
    HZ_ASSERT(Renderer::getApi() == RendererAPI::API::OpenGL, DefaultCoreHandler, Hazel::Enforce,
              "OpenGLTexture2D requested but RendererAPI::API != OpenGL");
    return std::make_shared<OpenGLTexture2D>(path);
}

} // namespace Hazel
