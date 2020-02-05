#pragma once

namespace Hazel
{
class Shader {
public:
    Shader(const std::string& vertex_src, const std::string& fragment_src);
    Shader(Shader const&) noexcept = default;
    Shader(Shader&&) noexcept = default;
    Shader& operator=(Shader const&) noexcept = default;
    Shader& operator=(Shader&&) noexcept = default;
    ~Shader() noexcept;

    void bind() const;
    void unbind() const;

private:
    std::uint32_t renderer_id_;
};
} // namespace Hazel
