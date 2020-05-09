#include "OpenGLTexture.h"

#include <utility>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Hazel/Core/AssertionHandler.h"

namespace Hazel {

OpenGLTexture2D::OpenGLTexture2D(std::string path) : path_{std::move(path)}
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data{stbi_load(path_.c_str(), &width, &height, &channels, 0)};
    HZ_ASSERT(data != nullptr, DefaultCoreHandler, Hazel::Enforce, "Failed to load image");
    width_ = width;
    height_ = height;

    HZ_ASSERT(channels == 3 || channels == 4, DefaultCoreHandler, Hazel::Enforce,
              "Unsupported texture format");

    const GLenum internal_format = [channels]() noexcept {
        if (channels == 4) { return GL_RGBA8; }
        return GL_RGB8;
    }();

    const GLenum data_format = [channels]() noexcept {
        if (channels == 4) { return GL_RGBA; }
        return GL_RGB;
    }();

    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
    glTextureStorage2D(renderer_id_, 1, internal_format, width_, height_);

    glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() noexcept { glDeleteTextures(1, &renderer_id_); }

void OpenGLTexture2D::bind(std::uint32_t slot) const { glBindTextureUnit(slot, renderer_id_); }

}  // namespace Hazel
