#pragma once

#include <memory>

#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
class VertexArray {
public:
    VertexArray() noexcept = default;
    virtual ~VertexArray() = default;
    VertexArray& operator=(VertexArray&&) = delete;

    static Scope<VertexArray> create();

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(Scope<VertexBuffer>) = 0;
    virtual void setIndexBuffer(Scope<IndexBuffer>) = 0;
    virtual std::vector<Scope<VertexBuffer>> const& getVertexBuffers() const noexcept = 0;
    virtual IndexBuffer const& getIndexBuffer() const noexcept = 0;
};
} // namespace Hazel
