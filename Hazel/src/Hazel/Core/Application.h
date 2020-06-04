#pragma once

#include <chrono>

#include "Hazel/Core/Base.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/Event.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel {

class HAZEL_API Application {
public:
    Application();
    virtual ~Application();

    void run();

    void onEvent(Event& e);

    void pushLayer(std::unique_ptr<Layer> layer);
    void pushOverlay(std::unique_ptr<Layer> overlay);

    void close() noexcept;

    inline Window& getWindow() noexcept { return *window_; }
    inline Window const& getWindow() const noexcept { return *window_; }

    static inline Application& get() noexcept { return *instance_; }

private:
    bool onWindowClose(WindowCloseEvent& e) noexcept;
    bool onWindowResize(WindowResizeEvent& e) noexcept;
    void initGLData() noexcept;

    std::unique_ptr<Window> window_;
    bool running_{true};
    Timestep last_frame_time_;
    bool minimized_{false};
    ImGuiLayer* imgui_layer_;
    LayerStack layerStack_;

    // --- static member data
    static Application* instance_;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Hazel
