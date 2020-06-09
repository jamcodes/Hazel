#pragma once

#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel
{
class OpenGLFramebuffer final : public Framebuffer {
public:
    explicit OpenGLFramebuffer(FramebufferSpecification const& spec);
    ~OpenGLFramebuffer() override;

    void recreate();

    void bind() noexcept override;
    void unbind() noexcept override;

    std::uint32_t getColorAttachmentRendererId() const noexcept override { return color_attachment_; }

    FramebufferSpecification const& getSpecification() const noexcept override { return spec_; }
private:

    std::uint32_t renderer_id_;
    std::uint32_t color_attachment_;
    std::uint32_t depth_attachment_;
    FramebufferSpecification spec_;
};
} // namespace Hazel
