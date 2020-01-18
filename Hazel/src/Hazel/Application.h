#pragma once

#include "Core.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/Event.h"
#include "Hazel/LayerStack.h"
#include "Window.h"

namespace Hazel {

class HAZEL_API Application {
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void pushLayer(std::unique_ptr<Layer> layer);
    void pushOverlay(std::unique_ptr<Layer> overlay);

private:
    bool OnWindowClose(WindowCloseEvent& e) noexcept;
    std::unique_ptr<Window> window_;
    bool running_{true};
    LayerStack layerStack_;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Hazel
