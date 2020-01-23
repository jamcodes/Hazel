#pragma once

#include "Event.h"

namespace Hazel {

    class HAZEL_API MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : mouseX_(x), mouseY_(y) {}

        inline float getX() const { return mouseX_; }
        inline float getY() const { return mouseY_; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << mouseX_ << ", " << mouseY_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float mouseX_, mouseY_;
    };

    class HAZEL_API MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : xOffset_{xOffset}, yOffset_{yOffset} {}

        inline float getXOffset() const { return xOffset_; }
        inline float getYOffset() const { return yOffset_; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float xOffset_, yOffset_;
    };

    class HAZEL_API MouseButtonEvent : public Event
    {
    public:
        inline int getMouseButton() const { return button_; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(int button)
            : button_{button} {}

        int button_;
    };

    class HAZEL_API MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent{button} {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << button_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class HAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent{button} {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << button_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}
