#include "OpenGLTexture.h"

#include <stb/stb_image.h>

#include <utility>

#include "Hazel/Core/AssertionHandler.h"

namespace Hazel {

OpenGLTexture2D::OpenGLTexture2D(unsigned width, unsigned height)
    : width_{width}, height_{height}, internal_format_{GL_RGBA8}, data_format_{GL_RGBA}
{
    HZ_PROFILE_FUNCTION();

    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
    glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

    glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(std::string path) : path_{std::move(path)}
{
    HZ_PROFILE_FUNCTION();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data{[&]() noexcept {
        HZ_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D(const std::string&) -> stbi_load");
        return stbi_load(path_.c_str(), &width, &height, &channels, 0);
    }()};
    HZ_ASSERT(data != nullptr, DefaultCoreHandler, Hazel::Enforce, "Failed to load image");
    width_ = width;
    height_ = height;

    HZ_ASSERT(channels == 3 || channels == 4, DefaultCoreHandler, Hazel::Enforce, "Unsupported texture format");

    internal_format_ = [channels]() noexcept {
        if (channels == 4) {
            return GL_RGBA8;
        }
        return GL_RGB8;
    }();

    data_format_ = [channels]() noexcept {
        if (channels == 4) {
            return GL_RGBA;
        }
        return GL_RGB;
    }();

    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
    glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

    glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() noexcept
{
    HZ_PROFILE_FUNCTION();
    glDeleteTextures(1, &renderer_id_);
}

void OpenGLTexture2D::bind(std::uint32_t slot) const
{
    HZ_PROFILE_FUNCTION();
    glBindTextureUnit(slot, renderer_id_);
}

void OpenGLTexture2D::setData(const void* data, unsigned size) noexcept
{
    HZ_PROFILE_FUNCTION();
    const auto bytes_per_pixel{data_format_ == GL_RGBA ? 4 : 3};
    HZ_ASSERT(size == width_ * height_ * bytes_per_pixel, DefaultCoreHandler, Hazel::Enforce,
              "Data must be entire texture");
    glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
}

}  // namespace Hazel
