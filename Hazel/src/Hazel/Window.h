#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

struct WindowProps {
    std::string title{"Hazel Engine"};
    unsigned width{1280};
    unsigned height{720};
};

// Interface representing a desktop system based Window
class HAZEL_API Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;
    Window& operator=(Window&&) = delete;

    virtual void onUpdate() = 0;

    virtual unsigned getWidth() const = 0;
    virtual unsigned getHeight() const = 0;

    // Window attributes
    virtual void setEventCallback(EventCallbackFn cb) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSync() const = 0;

    virtual void* getNativeWindow() const noexcept = 0;

    static std::unique_ptr<Window> create(const WindowProps& props = WindowProps());
};

}  // namespace Hazel
