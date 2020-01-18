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
    static EventType GetStaticType() { return EventType::##type; }              \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }

class HAZEL_API Event {
public:
    bool handled{false};

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }
    virtual ~Event() = default;

    inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
};

class EventDispatcher {
public:
    explicit EventDispatcher(Event& event) noexcept : event_(event) {}
    ~EventDispatcher() noexcept = default;
    EventDispatcher& operator=(EventDispatcher&&) noexcept = delete;

    template <typename EventT, typename EventFunction>
    bool dispatch(EventFunction&& f) noexcept(std::is_nothrow_invocable_v<EventFunction, EventT&>)
    {
        if (event_.GetEventType() == EventT::GetStaticType())
        {
            event_.handled = f(static_cast<EventT&>(event_));
            return true;
        }
        return false;
    }

private:
    Event& event_;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }
}  // namespace Hazel
