#pragma once

#include "Core.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/Event.h"
#include "Hazel/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Window.h"

namespace Hazel {

class HAZEL_API Application {
public:
    Application();
    virtual ~Application();

    void run();

    void onEvent(Event& e);

    void pushLayer(std::unique_ptr<Layer> layer);
    void pushOverlay(std::unique_ptr<Layer> overlay);

    inline Window& getWindow() noexcept { return *window_; }
    inline Window const& getWindow() const noexcept { return *window_; }

    static inline Application& get() noexcept { return *instance_; }

private:
    bool onWindowClose(WindowCloseEvent& e) noexcept;
    void initGLData() noexcept;

    std::unique_ptr<Window> window_;
    bool running_{true};
    ImGuiLayer* imgui_layer_;
    LayerStack layerStack_;

    unsigned int vertex_array_;
    unsigned int vertex_buffer_;
    unsigned int index_buffer_;

// --- static member data
    static Application* instance_;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Hazel
