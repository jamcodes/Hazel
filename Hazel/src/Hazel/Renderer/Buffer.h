#pragma once

#include <cstdint>

namespace Hazel {

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;
    VertexBuffer& operator=(VertexBuffer&&) = delete;

    template <typename Container>
    static std::unique_ptr<VertexBuffer> create(Container const& vertices)
    {
        static_assert(std::is_same_v<Container::value_type, float>,
                      "\n\tVertexBuffer may be constructed only from a container of floats\n");
        return create(vertices.data(), static_cast<std::uint32_t>(vertices.size()));
    }

    static std::unique_ptr<VertexBuffer> create(const float* vertices, std::uint32_t size);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
    IndexBuffer& operator=(IndexBuffer&&) = delete;

    template <typename Container>
    static std::unique_ptr<IndexBuffer> create(Container const& indices)
    {
        static_assert(
            std::is_same_v<Container::value_type, std::uint32_t>,
            "\n\tIndexBuffer may be constructed only from a container of unsigned integers\n");
        return create(indices.data(), static_cast<std::uint32_t>(indices.size()));
    }

    static std::unique_ptr<IndexBuffer> create(const std::uint32_t* indices, std::uint32_t size);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual std::uint32_t getCount() const noexcept = 0;
};

}  // namespace Hazel
