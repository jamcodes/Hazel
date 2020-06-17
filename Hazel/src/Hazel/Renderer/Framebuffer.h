#pragma once

#include <Hazel/Core/Base.h>

namespace Hazel
{
struct FramebufferSpecification
{
    std::uint32_t width, height;
    std::uint32_t samples{1};
    bool swapchain_target{false};
};

class Framebuffer {
public:
    Framebuffer() noexcept = default;
    Framebuffer(Framebuffer const&) = default;
    Framebuffer(Framebuffer&&) noexcept = default;
    Framebuffer& operator=(Framebuffer const&) = default;
    Framebuffer& operator=(Framebuffer&&) noexcept = default;

    virtual ~Framebuffer() = default;

    static Ref<Framebuffer> create(FramebufferSpecification const& spec);

    virtual void resize(std::uint32_t width, std::uint32_t height) = 0;

    virtual void bind() noexcept = 0;
    virtual void unbind() noexcept = 0;

    virtual std::uint32_t getColorAttachmentRendererId() const noexcept = 0;

    virtual FramebufferSpecification const& getSpecification() const noexcept = 0;
    // virtual FramebufferSpecification& getSpecification() noexcept = 0;

};
} // namespace Hazel
