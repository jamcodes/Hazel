#pragma once

#include <utility>

#include "Hazel/Core.h"
#include "Hazel/KeyCodes.h"

namespace Hazel {
class HAZEL_API Input {
public:
    Input() = default;
    virtual ~Input() noexcept = default;
    Input& operator=(Input&&) noexcept = delete;

    inline static bool isKeyPressed(int keycode) { return s_instance_->isKeyPressedImpl(keycode); }
    inline static bool isKeyPressed(KeyCode keycode)
    {
        return s_instance_->isKeyPressedImpl(toInteger<int>(keycode));
    }

    inline static bool isMouseButtonPressed(int button)
    {
        return s_instance_->isMouseButtonPressedImpl(button);
    }

    inline static float getMouseX() { return s_instance_->getMouseXImpl(); }
    inline static float getMouseY() { return s_instance_->getMouseYImpl(); }

    inline static std::pair<float, float> getMousePosition()
    {
        return s_instance_->getMousePositionImpl();
    }

protected:
    virtual bool isKeyPressedImpl(int keycode) const = 0;
    virtual bool isMouseButtonPressedImpl(int button) const = 0;
    virtual float getMouseXImpl() const = 0;
    virtual float getMouseYImpl() const = 0;
    virtual std::pair<float, float> getMousePositionImpl() const = 0;

private:
    static Input* s_instance_;
};
}  // namespace Hazel
