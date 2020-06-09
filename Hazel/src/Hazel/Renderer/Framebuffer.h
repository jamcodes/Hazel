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
    virtual ~Framebuffer() = default;

    static Ref<Framebuffer> create(FramebufferSpecification const& spec);

    virtual void bind() noexcept = 0;
    virtual void unbind() noexcept = 0;

    virtual std::uint32_t getColorAttachmentRendererId() const noexcept = 0;

    virtual FramebufferSpecification const& getSpecification() const noexcept = 0;
    // virtual FramebufferSpecification& getSpecification() noexcept = 0;

};
} // namespace Hazel
