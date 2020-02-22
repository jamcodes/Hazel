#pragma once

#include <glm/glm.hpp>

namespace Hazel {
class Shader {
public:
    virtual ~Shader() noexcept = default;
    Shader& operator=(Shader&&) noexcept = delete;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    template <typename T>
    static std::unique_ptr<T> create(const std::string& vertex_src,
                                     const std::string& fragment_src);

    static std::unique_ptr<Shader> create(const std::string& vertex_src,
                                          const std::string& fragment_src);
};
}  // namespace Hazel
