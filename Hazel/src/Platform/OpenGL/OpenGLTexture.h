#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(std::string path);
    ~OpenGLTexture2D() noexcept override;
    OpenGLTexture2D& operator=(OpenGLTexture2D&&) = delete;

    std::uint32_t getWidth() const noexcept override { return width_; }
    std::uint32_t getHeight() const noexcept override { return height_; }

    void bind(std::uint32_t slot) const override;

private:
    std::uint32_t renderer_id_;
    std::uint32_t width_;
    std::uint32_t height_;
    std::string path_;
};
} // namespace Hazel
