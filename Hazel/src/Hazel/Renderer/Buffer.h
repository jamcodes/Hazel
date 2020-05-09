#pragma once

#include <cstdint>
#include <type_traits>

#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Core/Core.h"

namespace Hazel {

enum class ShaderDataType {
    None,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool,
};

constexpr std::uint32_t shaderDataTypeSize(ShaderDataType type) noexcept
{
    // clang-format off
    switch(type) {
        case ShaderDataType::None  :    return static_cast<decltype(sizeof(int))>(0);
        case ShaderDataType::Float :    return sizeof(float);
        case ShaderDataType::Float2:    return sizeof(float) * 2;
        case ShaderDataType::Float3:    return sizeof(float) * 3;
        case ShaderDataType::Float4:    return sizeof(float) * 4;
        case ShaderDataType::Mat3  :    return sizeof(float) * 3 * 3;
        case ShaderDataType::Mat4  :    return sizeof(float) * 4 * 4;
        case ShaderDataType::Int   :    return sizeof(unsigned int);
        case ShaderDataType::Int2  :    return sizeof(unsigned int) * 2;
        case ShaderDataType::Int3  :    return sizeof(unsigned int) * 3;
        case ShaderDataType::Int4  :    return sizeof(unsigned int) * 4;
        case ShaderDataType::Bool  :    return sizeof(bool);
    }
    // clang-format on
    HZ_ASSERT(false, DefaultCoreHandler, Enforce, "Unknown ShaderDataType");
    return 0;
}

template <typename PlatformEnumType>
constexpr PlatformEnumType shaderDataTypeToPlatformType(ShaderDataType type) noexcept
{
    static_assert(sizeof(PlatformEnumType) == 0,
                  "Platform-specific implementation of shaderDataTypeToPlatformType must be "
                  "provided");
    return PlatformEnumType{};
}

constexpr std::uint32_t componentCount(ShaderDataType type) noexcept
{
    // clang-format off
    switch(type) {
        case ShaderDataType::None  :    return 0;
        case ShaderDataType::Float :    return 1;
        case ShaderDataType::Float2:    return 2;
        case ShaderDataType::Float3:    return 3;
        case ShaderDataType::Float4:    return 4;
        case ShaderDataType::Mat3  :    return 3 * 3;
        case ShaderDataType::Mat4  :    return 4 * 4;
        case ShaderDataType::Int   :    return 1;
        case ShaderDataType::Int2  :    return 2;
        case ShaderDataType::Int3  :    return 3;
        case ShaderDataType::Int4  :    return 4;
        case ShaderDataType::Bool  :    return 1;
    }
    // clang-format on
    HZ_ASSERT(false, DefaultCoreHandler, Enforce, "Unknown ShaderDataType");
    return 0;
}

struct BufferElement {
    template <typename StringT,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<StringT>, BufferElement> &&
                                          std::is_convertible_v<StringT, std::string>>>
    BufferElement(ShaderDataType type_, StringT&& name_)
        : name{std::forward<StringT>(name_)}, type{type_}, size{shaderDataTypeSize(type)}
    {
    }

    template <typename StringT,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<StringT>, BufferElement> &&
                                          std::is_convertible_v<StringT, std::string>>>
    BufferElement(ShaderDataType type_, StringT&& name_, bool normalized_)
        : BufferElement{type_, std::forward<StringT>(name_)}, normalized{normalized_}
    {
    }

    BufferElement() = default;
    BufferElement(BufferElement const&) = default;
    BufferElement(BufferElement&&) noexcept = default;
    BufferElement& operator=(BufferElement const&) = default;
    BufferElement& operator=(BufferElement&&) noexcept = default;
    ~BufferElement() = default;

    std::string name{};
    ShaderDataType type{ShaderDataType::None};
    std::uint32_t size{0};
    std::uint32_t offset{0};
    bool normalized{false};
};

class BufferLayout {
public:
    using value_type = BufferElement;
    using reference = typename std::vector<value_type>::reference;
    using const_reference = typename std::vector<value_type>::const_reference;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;

    BufferLayout(std::initializer_list<BufferElement> elements) : elements_(std::move(elements))
    {
        calculateOffsetAndStride();
    }

    BufferLayout() = default;
    BufferLayout(BufferLayout const&) = default;
    BufferLayout(BufferLayout&&) noexcept = default;
    BufferLayout& operator=(BufferLayout const&) = default;
    BufferLayout& operator=(BufferLayout&&) noexcept = default;
    ~BufferLayout() = default;

    inline const std::vector<BufferElement>& getElements() const noexcept { return elements_; }
    inline auto getStride() const noexcept { return stride_; }

    iterator begin() noexcept { return elements_.begin(); }
    iterator end() noexcept { return elements_.end(); }
    const_iterator begin() const noexcept { return elements_.begin(); }
    const_iterator end() const noexcept { return elements_.end(); }
    const_iterator cbegin() const noexcept { return elements_.cbegin(); }
    const_iterator cend() const noexcept { return elements_.cend(); }

private:
    void calculateOffsetAndStride() noexcept
    {
        std::uint32_t offset{0};
        stride_ = 0;
        for (auto& e : elements_) {
            e.offset = offset;
            offset += e.size;
            stride_ += e.size;
        }
    }

    std::vector<BufferElement> elements_;
    std::uint32_t stride_{0};
};

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;
    VertexBuffer& operator=(VertexBuffer&&) = delete;

    template <typename Container>
    static auto create(Container const& vertices)
    {
        static_assert(std::is_same_v<Container::value_type, float>,
                      "\n\tVertexBuffer may be constructed only from a container of floats\n");
        return create(vertices.data(), static_cast<std::uint32_t>(vertices.size()));
    }

    template <typename Container>
    static auto create(Container const& vertices, BufferLayout const& layout)
    {
        static_assert(std::is_same_v<Container::value_type, float>,
                      "\n\tVertexBuffer may be constructed only from a container of floats\n");
        auto buffer{create(vertices)};
        buffer->setLayout(layout);
        return buffer;
    }

    static Scope<VertexBuffer> create(const float* vertices, std::uint32_t size);

    virtual const BufferLayout& getLayout() const noexcept = 0;
    virtual void setLayout(BufferLayout const&) = 0;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
    IndexBuffer& operator=(IndexBuffer&&) = delete;

    template <typename Container>
    static Scope<IndexBuffer> create(Container const& indices)
    {
        static_assert(
            std::is_same_v<Container::value_type, std::uint32_t>,
            "\n\tIndexBuffer may be constructed only from a container of unsigned integers\n");
        return create(indices.data(), static_cast<std::uint32_t>(indices.size()));
    }

    static Scope<IndexBuffer> create(const std::uint32_t* indices, std::uint32_t size);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual std::uint32_t getCount() const noexcept = 0;
};

}  // namespace Hazel
