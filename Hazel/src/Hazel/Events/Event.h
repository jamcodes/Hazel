#pragma once

#include "Hazel/Core.h"

namespace Hazel {

// Events in Hazel are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

enum class EventType {
    None = 0,

    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,

    AppTick,
    AppUpdate,
    AppRender,

    KeyTyped,
    KeyPressed,
    KeyReleased,

    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory {
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType getStaticType() { return EventType::##type; }              \
    virtual EventType getEventType() const override { return getStaticType(); } \
    virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int getCategoryFlags() const override { return category; }

class HAZEL_API Event {
public:
    bool handled{false};

    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual int getCategoryFlags() const = 0;
    virtual std::string toString() const { return getName(); }
    virtual ~Event() = default;

    inline bool isInCategory(EventCategory category) { return getCategoryFlags() & category; }
};

class EventDispatcher {
public:
    explicit EventDispatcher(Event& event) noexcept : event_(event) {}
    ~EventDispatcher() noexcept = default;
    EventDispatcher& operator=(EventDispatcher&&) noexcept = delete;

    template <typename EventT, typename EventFunction>
    bool dispatch(EventFunction&& f) noexcept(std::is_nothrow_invocable_v<EventFunction, EventT&>)
    {
        if (event_.getEventType() == EventT::getStaticType())
        {
            event_.handled = f(static_cast<EventT&>(event_));
            return true;
        }
        return false;
    }

private:
    Event& event_;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.toString(); }
}  // namespace Hazel
