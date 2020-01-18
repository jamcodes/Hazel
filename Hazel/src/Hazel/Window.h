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

    virtual void OnUpdate() = 0;

    virtual unsigned GetWidth() const = 0;
    virtual unsigned GetHeight() const = 0;

    // Window attributes
    virtual void SetEventCallback(EventCallbackFn cb) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
};

}  // namespace Hazel
