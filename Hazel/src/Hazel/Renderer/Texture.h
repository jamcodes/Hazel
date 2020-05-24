#pragma once

#include <cstdint>
#include <string>

#include "Hazel/Core/Core.h"

namespace Hazel
{
class Texture {
public:
    virtual ~Texture() noexcept = default;
    Texture& operator=(Texture&&) = delete;

    virtual std::uint32_t getWidth() const noexcept = 0;
    virtual std::uint32_t getHeight() const noexcept = 0;

    // upload a raw block of memory to the gpu
    virtual void setData(const void*, unsigned size) = 0;

    virtual void bind(std::uint32_t slot = 0) const = 0;

    [[nodiscard]] friend inline bool operator==(Texture const& l, Texture const& r) noexcept
    {
        return l.do_equals(r);
    }

private:
    virtual bool do_equals(Texture const& other) const noexcept = 0;
};

class Texture2D : public Texture {
public:
    ~Texture2D() noexcept override = default;
    Texture2D& operator=(Texture2D&&) = delete;

    template<typename TextureT>
    static Ref<TextureT> create(const std::string& path);
    static Ref<Texture2D> create(const std::string& path);
    template<typename TextureT>
    static Ref<TextureT> create(unsigned width, unsigned height);
    static Ref<Texture2D> create(unsigned width, unsigned height);
};
} // namespace Hazel
