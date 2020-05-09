#pragma once

#include "Hazel/Events/Event.h"

namespace Hazel {

class HAZEL_API WindowResizeEvent : public Event {
public:
    WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height) {}

    inline unsigned int getWidth() const { return width_; }
    inline unsigned int getHeight() const { return height_; }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << width_ << ", " << height_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int width_, height_;
};

class HAZEL_API WindowCloseEvent : public Event {
public:
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class HAZEL_API AppTickEvent : public Event {
public:
    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class HAZEL_API AppUpdateEvent : public Event {
public:
    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class HAZEL_API AppRenderEvent : public Event {
public:
    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}  // namespace Hazel
