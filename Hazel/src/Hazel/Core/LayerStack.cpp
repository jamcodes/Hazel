#include "LayerStack.h"

namespace Hazel {
void LayerStack::pushLayer(value_type layer)
{
    auto& l{*layer};
    layers_.emplace(std::next(layers_.cbegin(), layer_index_), std::move(layer));
    ++layer_index_;
    l.onAttach();
}

void LayerStack::pushOverlay(value_type overlay) {
    auto& o{*overlay};
    layers_.emplace_back(std::move(overlay));
    o.onAttach();
}

void LayerStack::popLayer(Layer const* const layer)
{
    auto it =
        std::find_if(layers_.cbegin(),
                     std::next(layers_.cbegin(), layer_index_), [layer](auto const& p) noexcept {
                         return p.get() == layer;
                     });
    if (it != layers_.cend()) {
        (*it)->onDetach();
        layers_.erase(it);
        --layer_index_;
    }
}

void LayerStack::popOverlay(Layer const* const overlay)
{
    auto it = std::find_if(layers_.cbegin(), layers_.cend(), [overlay](auto const& p) noexcept {
        return p.get() == overlay;
    });
    if (it != layers_.cend()) {
        (*it)->onDetach();
        layers_.erase(it);
    }
}

}  // namespace Hazel
