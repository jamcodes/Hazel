#include "OpenglFramebuffer.h"

#include <glad/glad.h>

#include "Hazel/Core/AssertionHandler.h"

namespace Hazel {
OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecification const& spec) : spec_{spec} { recreate(); }

OpenGLFramebuffer::~OpenGLFramebuffer() { glDeleteFramebuffers(1, &renderer_id_); }

void OpenGLFramebuffer::recreate()
{
    glCreateFramebuffers(1, &renderer_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

    glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment_);
    glBindTexture(GL_TEXTURE_2D, color_attachment_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec_.width, spec_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &depth_attachment_);
    glBindTexture(GL_TEXTURE_2D, depth_attachment_);
    // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec_.width, spec_.height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, spec_.width, spec_.height, 0,
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_, 0);

    HZ_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Incomplete Framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind() noexcept { glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_); }

void OpenGLFramebuffer::unbind() noexcept { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

}  // namespace Hazel
