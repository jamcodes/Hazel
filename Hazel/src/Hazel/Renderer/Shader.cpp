#include "Shader.h"

#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

Scope<Shader> Shader::create(const std::string& filepath)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLShader>(filepath);
    default:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

template<>
Scope<OpenGLShader> Shader::create(const std::string& filepath)
{
    HZ_EXPECTS(Renderer::getApi() == RendererAPI::API::OpenGL, DefaultCoreHandler, Hazel::Enforce,
    "OpenGLShader was requested, but RendererAPI != OpenGL");
    return std::make_unique<OpenGLShader>(filepath);
}


Scope<Shader> Shader::create(std::string const& name, const std::string& vertex_src,
                                       const std::string& fragment_src)
{
    switch (Renderer::getApi()) {
    case RendererAPI::API::None:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce,
                  "RendererAPI::API::None is currently not supported");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLShader>(name, vertex_src, fragment_src);
    default:
        HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce, "Unknown RendererAPI::API");
    }

    return nullptr;
}

template<>
Scope<OpenGLShader>
Shader::create<OpenGLShader>(std::string const& name, const std::string& vertex_src, const std::string& fragment_src)
{
    HZ_EXPECTS(Renderer::getApi() == RendererAPI::API::OpenGL, DefaultCoreHandler, Hazel::Enforce,
    "OpenGLShader was requested, but RendererAPI != OpenGL");
    return std::make_unique<OpenGLShader>(name, vertex_src, fragment_src);
}

// --- ShaderLibrary

void ShaderLibrary::add(const Ref<Shader>& shader)
{
    auto&& name{shader->getName()};
    add(name, shader);
}

void ShaderLibrary::add(std::string const& name, const Ref<Shader>& shader)
{
    auto const res{shaders_.insert({name, shader})};
    HZ_EXPECTS(res.second == true, DefaultCoreHandler, Hazel::Enforce, "Shader already exists");
}

Ref<Shader> ShaderLibrary::load(const std::string& filepath)
{
    Ref<Shader> shader{Shader::create(filepath)};
    add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
{
    Ref<Shader> shader{Shader::create(filepath)};
    add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::get(const std::string& name) const
{
    auto const it{shaders_.find(name)};
    HZ_EXPECTS(it != shaders_.end(), DefaultCoreHandler, Hazel::Enforce, "Shader not found");
    return it->second;
}

inline bool ShaderLibrary::exists(std::string const& name) const noexcept
{
    return std::as_const(shaders_).find(name) != shaders_.cend();
}

}  // namespace Hazel
