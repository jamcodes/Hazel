#include "SubTexture2D.h"

namespace Hazel {
SubTexture2D::SubTexture2D(Ref<Texture2D> const& texture, glm::vec2 min, glm::vec2 max)
    : texture_{texture},
      tex_coords_{glm::vec2{min.x, min.y}, glm::vec2{max.x, min.y}, glm::vec2{max.x, max.y}, glm::vec2{min.x, max.y}}
{
}

Ref<SubTexture2D> SubTexture2D::createFromCoords(Ref<Texture2D> const& texture, glm::vec2 coords, glm::vec2 cell_size,
                                                 glm::vec2 sprite_size)
{
    glm::vec2 min{(coords.x * cell_size.x) / texture->getWidth(), (coords.y * cell_size.y) / texture->getHeight()};
    glm::vec2 max{((coords.x + sprite_size.x) * cell_size.x) / texture->getWidth(),
                  ((coords.y + sprite_size.y) * cell_size.y) / texture->getHeight()};
    return makeRef<SubTexture2D>(texture, std::move(min), std::move(max));
}

}  // namespace Hazel
