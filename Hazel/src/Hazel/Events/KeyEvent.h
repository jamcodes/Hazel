#pragma once

#include "Hazel/Events/Event.h"

namespace Hazel {

    class HAZEL_API KeyEvent : public Event
    {
    public:
        inline int getKeyCode() const noexcept { return keyCode_; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        explicit KeyEvent(int keycode) noexcept
            : keyCode_(keycode) {}

        int keyCode_;
    };

    class HAZEL_API KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(int keycode) : KeyEvent{keycode} { }

        std::string toString() const override
        {
            std::ostringstream ss{"KeyTypedEvent: ", std::ios_base::ate};
            ss << keyCode_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped);
    };

    class HAZEL_API KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent{keycode}, repeatCount_{repeatCount} {}

        inline int getRepeatCount() const { return repeatCount_; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode_ << " (" << repeatCount_ << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int repeatCount_;
    };

    class HAZEL_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent{keycode} {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << keyCode_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}
