#pragma once

#include "EnumOperators.h"

namespace Hazel {

enum class MouseButton {
    _1 = 0,
    _2 = 1,
    _3 = 2,
    _4 = 3,
    _5 = 4,
    _6 = 5,
    _7 = 6,
    _8 = 7,
    End = 8,
    Left = _1,
    Right = _2,
    Middle = _3,
};

template <>
struct enableEnumOperators<MouseButton> {
    static constexpr bool value{true};
};

}  // namespace Hazel
#define GLFW_MOUSE_BUTTON_1 0
#define GLFW_MOUSE_BUTTON_2 1
#define GLFW_MOUSE_BUTTON_3 2
#define GLFW_MOUSE_BUTTON_4 3
#define GLFW_MOUSE_BUTTON_5 4
#define GLFW_MOUSE_BUTTON_6 5
#define GLFW_MOUSE_BUTTON_7 6
#define GLFW_MOUSE_BUTTON_8 7
#define GLFW_MOUSE_BUTTON_LAST GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_3
