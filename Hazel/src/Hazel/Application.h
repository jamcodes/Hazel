#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Window.h"

namespace Hazel {

class HAZEL_API Application {
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

private:
    bool OnWindowClose(WindowCloseEvent& e) noexcept;
    std::unique_ptr<Window> window_;
    bool running_{true};
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Hazel
