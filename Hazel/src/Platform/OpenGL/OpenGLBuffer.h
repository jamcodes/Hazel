#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {
class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const float* vertices, const std::uint32_t size);
    ~OpenGLVertexBuffer() override;
    OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&&) = delete;

    const BufferLayout& getLayout() const noexcept override { return layout_; }
    void setLayout(BufferLayout const& layout) override { layout_ = layout; }
    void bind() const noexcept override;
    void unbind() const noexcept override;

private:
    std::uint32_t renderer_id_;
    BufferLayout layout_;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(const std::uint32_t* indices, const std::uint32_t size);
    ~OpenGLIndexBuffer() override;
    OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&&) = delete;

    void bind() const noexcept override;
    void unbind() const noexcept override;
    std::uint32_t getCount() const noexcept override { return count_; }

private:
    std::uint32_t renderer_id_;
    std::uint32_t count_;
};
}  // namespace Hazel
