#pragma once

#include <glad/glad.h>

#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(unsigned width, unsigned height);
    OpenGLTexture2D(std::string path);
    ~OpenGLTexture2D() noexcept override;
    OpenGLTexture2D& operator=(OpenGLTexture2D&&) = delete;

    std::uint32_t getWidth() const noexcept override final { return width_; }
    std::uint32_t getHeight() const noexcept override final { return height_; }
    std::uint32_t getRendererId() const noexcept override final { return renderer_id_; }


    void setData(const void* data, unsigned size) noexcept override;

    void bind(std::uint32_t slot) const override;

private:
    bool do_equals(Texture const& other) const noexcept final
    {
        return renderer_id_ == static_cast<OpenGLTexture2D const&>(other).renderer_id_;
    }

    std::uint32_t renderer_id_;
    std::uint32_t width_;
    std::uint32_t height_;
    GLenum internal_format_;
    GLenum data_format_;
    std::string path_;
};
} // namespace Hazel
