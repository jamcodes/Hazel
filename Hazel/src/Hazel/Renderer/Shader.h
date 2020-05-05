#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Hazel/Core.h"

namespace Hazel {

class Shader {
public:
    virtual ~Shader() noexcept = default;
    Shader& operator=(Shader&&) noexcept = delete;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual const std::string& getName() const noexcept = 0;

    template<typename ShaderT>
    static Scope<ShaderT> create(const std::string& filepath);
    static Scope<Shader> create(const std::string& filepath);

    template <typename ShaderT>
    static Scope<ShaderT> create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
    static Scope<Shader> create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
};

class ShaderLibrary {
public:

    void add(const Ref<Shader>& shader);
    void add(std::string const& name, const Ref<Shader>& shader);
    Ref<Shader> load(const std::string& filepath);
    Ref<Shader> load(const std::string& name, const std::string& filepath);

    Ref<Shader> get(const std::string& name) const;
    bool exists(std::string const& name) const noexcept;

private:
    std::unordered_map<std::string, Ref<Shader>> shaders_;
};
}  // namespace Hazel
