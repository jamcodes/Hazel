#include "Shader.h"

#include "Hazel/AssertionHandler.h"
#include "Hazel/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

Scope<Shader> Shader::create(const std::string& vertex_src,
                                       const std::string& fragment_src)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLShader>(vertex_src, fragment_src);
    default:
        HZ_ASSERT(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

template<>
Scope<OpenGLShader>
Shader::create<OpenGLShader>(const std::string& vertex_src, const std::string& fragment_src)
{
    HZ_ASSERT(Renderer::getApi() == RendererAPI::API::OpenGL, DefaultCoreHandler, Hazel::Enforce,
    "OpenGLShader was requested, but RendererAPI != OpenGL");
    return std::make_unique<OpenGLShader>(vertex_src, fragment_src);
}

}  // namespace Hazel
