#pragma once

#include "Hazel/Core/AssertionHandler.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {
class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray() noexcept;
    ~OpenGLVertexArray() override;
    OpenGLVertexArray& operator=(OpenGLVertexArray&&) = delete;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(Scope<VertexBuffer>) override;
    void setIndexBuffer(Scope<IndexBuffer>) override;

    std::vector<Scope<VertexBuffer>> const& getVertexBuffers() const noexcept override { return vertex_buffers_; }
    IndexBuffer const& getIndexBuffer() const noexcept override {
        HZ_EXPECTS(index_buffer_ != nullptr, DefaultCoreHandler, Enforce, "Index buffer not set");
        return *index_buffer_;
    }

private:
    inline void defineVertexAttributeArray(BufferElement const& element, std::uint32_t stride) noexcept;

    std::uint32_t renderer_id_{0};
    std::uint32_t vertex_buffer_index_{0};
    std::vector<Scope<VertexBuffer>> vertex_buffers_{};
    Scope<IndexBuffer> index_buffer_{nullptr};
};
}  // namespace Hazel
