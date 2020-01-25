#include "LayerStack.h"

namespace Hazel {
void LayerStack::pushLayer(value_type layer)
{
    layers_.emplace(layers_.cbegin(), std::move(layer));
}

void LayerStack::pushOverlay(value_type overlay)
{
    layers_.emplace_back(std::move(overlay));
}

void LayerStack::popLayer(Layer const* const layer)
{
    auto it = std::find_if(layers_.cbegin(), layers_.cend(), [layer](auto const& p) noexcept {
        return p.get() == layer;
    });
    if (it != layers_.cend()) {
        layers_.erase(it);
    }
}

void LayerStack::popOverlay(Layer const* const overlay)
{
    auto it = std::find_if(layers_.cbegin(), layers_.cend(), [overlay](auto const& p) noexcept {
        return p.get() == overlay;
    });
    if (it != layers_.cend()) {
        layers_.erase(it);
    }
}

}  // namespace Hazel
