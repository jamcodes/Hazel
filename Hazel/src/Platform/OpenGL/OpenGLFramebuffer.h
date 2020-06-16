#pragma once

#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel
{
class OpenGLFramebuffer final : public Framebuffer {
public:
    explicit OpenGLFramebuffer(FramebufferSpecification const& spec);
    ~OpenGLFramebuffer() override;

    void resize(std::uint32_t width, std::uint32_t height) override;

    void bind() noexcept override;
    void unbind() noexcept override;

    std::uint32_t getColorAttachmentRendererId() const noexcept override { return color_attachment_; }

    FramebufferSpecification const& getSpecification() const noexcept override { return spec_; }
private:
    void create() noexcept;
    void destroy() noexcept;

    std::uint32_t renderer_id_{0};
    std::uint32_t color_attachment_{0};
    std::uint32_t depth_attachment_{0};
    FramebufferSpecification spec_;
};
} // namespace Hazel
