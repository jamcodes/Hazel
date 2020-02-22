#pragma once

#include <glm/glm.hpp>

#include <Hazel/Renderer/Shader.h>

namespace Hazel {
class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& vertex_src, const std::string& fragment_src);
    OpenGLShader(OpenGLShader const&) noexcept = default;
    OpenGLShader(OpenGLShader&&) noexcept = default;
    OpenGLShader& operator=(OpenGLShader const&) noexcept = default;
    OpenGLShader& operator=(OpenGLShader&&) noexcept = default;
    ~OpenGLShader() noexcept override;

    void bind() const override;
    void unbind() const override;

    void uploadUniform(std::string const& name, int value) const;

    void uploadUniform(std::string const& name, float value) const;
    void uploadUniform(std::string const& name, glm::vec2 const& values) const;
    void uploadUniform(std::string const& name, glm::vec3 const& values) const;
    void uploadUniform(std::string const& name, glm::vec4 const& values) const;

    void uploadUniform(std::string const& name, glm::mat3 const& uniform) const;
    void uploadUniform(std::string const& name, glm::mat4 const& uniform) const;

private:
    std::uint32_t renderer_id_;
};
}  // namespace Hazel
