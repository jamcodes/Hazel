#pragma once

#include <utility>

#include "Hazel/Core/Base.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"

namespace Hazel {
class HAZEL_API Input {
public:
    static bool isKeyPressed(int keycode) noexcept;
    static bool isKeyPressed(KeyCode keycode) noexcept;

    static bool isMouseButtonPressed(int button) noexcept;
    static bool isMouseButtonPressed(MouseButton button) noexcept;
    static std::pair<float, float> getMousePosition() noexcept;
    static float getMouseX() noexcept;
    static float getMouseY() noexcept;

};
}  // namespace Hazel
