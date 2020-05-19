#include "Texture.h"

#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {

Ref<Texture2D> Texture2D::create(const std::string& path)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce, "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return makeRef<OpenGLTexture2D>(path);
    default:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

template <>
inline Ref<OpenGLTexture2D> Texture2D::create<OpenGLTexture2D>(const std::string& path)
{
    HZ_EXPECTS(Renderer::getApi() == RendererAPI::API::OpenGL, DefaultCoreHandler, Hazel::Enforce,
              "OpenGLTexture2D requested but RendererAPI::API != OpenGL");
    return makeRef<OpenGLTexture2D>(path);
}

template <typename TextureT>
inline Ref<TextureT> Texture2D::create(unsigned width, unsigned height)
{
    HZ_EXPECTS(Renderer::getApi() == RendererAPI::API::OpenGL, DefaultCoreHandler, Hazel::Enforce,
              "OpenGLTexture2D requested but RendererAPI::API != OpenGL");
    return makeRef<OpenGLTexture2D>(width, height);
}

Ref<Texture2D> Texture2D::create(unsigned width, unsigned height)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce, "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        // return makeRef<OpenGLTexture2D>(width, height);
        return create<OpenGLTexture2D>(width, height);
    default:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }
}

}  // namespace Hazel
