#pragma once

#include <memory>
#include <vector>

#include "Hazel/Core.h"
#include "Layer.h"

namespace Hazel {

class HAZEL_API LayerStack {
public:
    using value_type = typename std::unique_ptr<Layer>;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;
    using reverse_itertor = typename std::vector<value_type>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<value_type>::const_reverse_iterator;

    LayerStack() = default;
    ~LayerStack() = default;
    LayerStack& operator=(LayerStack&&) noexcept = delete;

    void pushLayer(value_type);
    void pushOverlay(value_type);
    void popLayer(Layer const* const);
    void popOverlay(Layer const* const);

    inline iterator begin() noexcept { return layers_.begin(); }
    inline iterator end() noexcept { return layers_.end(); }
    inline const_iterator begin() const noexcept { return layers_.begin(); }
    inline const_iterator end() const noexcept { return layers_.end(); }
    inline const_iterator cbegin() const noexcept { return layers_.cbegin(); }
    inline const_iterator cend() const noexcept { return layers_.cend(); }
    inline reverse_itertor rbegin() noexcept { return layers_.rbegin(); }
    inline reverse_itertor rend() noexcept { return layers_.rend(); }
    inline const_reverse_iterator crbegin() const noexcept { return layers_.crbegin(); }
    inline const_reverse_iterator crend() const noexcept { return layers_.crend(); }

private:
    std::vector<value_type> layers_;
    int layer_index_{0};
};
}  // namespace Hazel
