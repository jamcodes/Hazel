#pragma once

#include <array>

#include <glm/glm.hpp>

#include "Texture.h"

namespace Hazel
{

class SubTexture2D
{
public:
    using iterator = std::array<glm::vec2, 4>::iterator;
    using const_iterator = std::array<glm::vec2, 4>::const_iterator;

    SubTexture2D(Ref<Texture2D> const& texture, glm::vec2 min, glm::vec2 max);

    static Ref<SubTexture2D> createFromCoords(Ref<Texture2D> const& texture, glm::vec2 coords, glm::vec2 cell_size, glm::vec2 sprite_size = {1, 1});

    Ref<Texture2D> const& getTexture() const noexcept { return texture_; }
    auto const& getCoords() const noexcept { return tex_coords_; }
    auto& getCoords() noexcept { return tex_coords_; }

    iterator begin() noexcept { return tex_coords_.begin(); }
    iterator end() noexcept { return tex_coords_.end(); }

    const_iterator begin() const noexcept { return tex_coords_.begin(); }
    const_iterator end() const noexcept { return tex_coords_.end(); }
    const_iterator cbegin() const noexcept { return tex_coords_.cbegin(); }
    const_iterator cend() const noexcept { return tex_coords_.cend(); }

    glm::vec2 operator[](unsigned i) const noexcept
    {
        HZ_ASSERT(i < 4, "Invalid SubTexture2D index");
        return tex_coords_[i];
    }

    glm::vec2& operator[](unsigned i) noexcept
    {
        HZ_ASSERT(i < 4, "Invalid SubTexture2D index");
        return tex_coords_[i];
    }
private:
    Ref<Texture2D> texture_;
    std::array<glm::vec2, 4> tex_coords_;
};

} // namespace Hazel
