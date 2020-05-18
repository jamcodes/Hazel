#include "LayerStack.h"

namespace Hazel {

LayerStack::~LayerStack()
{
    for(auto& layer : layers_){
        // TODO: move detaching into Application level code
        layer->onDetach();
    }
}
    
void LayerStack::pushLayer(value_type layer)
{
    layers_.emplace(std::next(layers_.cbegin(), layer_index_), std::move(layer));
    ++layer_index_;
}

void LayerStack::pushOverlay(value_type overlay) {
    layers_.emplace_back(std::move(overlay));
}

void LayerStack::popLayer(Layer const* const layer)
{
    auto it =
        std::find_if(layers_.cbegin(),
                     std::next(layers_.cbegin(), layer_index_), [layer](auto const& p) noexcept {
                         return p.get() == layer;
                     });
    if (it != layers_.cend()) {
        // TODO: move detaching into Application level code
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
        // TODO: move detaching into Application level code
        (*it)->onDetach();
        layers_.erase(it);
    }
}

}  // namespace Hazel
