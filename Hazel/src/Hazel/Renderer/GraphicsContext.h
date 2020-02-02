#pragma once

namespace Hazel {

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    GraphicsContext& operator=(GraphicsContext&&) noexcept = delete;

    virtual void swapBuffers() noexcept = 0;
};

}  // namespace Hazel
