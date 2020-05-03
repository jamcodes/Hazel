#pragma once

#include <glm/glm.hpp>

#include "Hazel/Core.h"

namespace Hazel {

class Shader {
public:
    virtual ~Shader() noexcept = default;
    Shader& operator=(Shader&&) noexcept = delete;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    template<typename ShaderT>
    static Scope<ShaderT> create(const std::string& filepath);
    static Scope<Shader> create(const std::string& filepath);

    template <typename ShaderT>
    static Scope<ShaderT> create(const std::string& vertex_src, const std::string& fragment_src);
    static Scope<Shader> create(const std::string& vertex_src, const std::string& fragment_src);
};
}  // namespace Hazel
