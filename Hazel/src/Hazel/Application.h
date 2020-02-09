#pragma once

#include "Core.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/Event.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/LayerStack.h"
#include "Window.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"


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

    std::unique_ptr<Shader> shader_;
    std::unique_ptr<VertexArray> tr_vertex_array_;

    std::unique_ptr<Shader> shader_sq_;
    std::unique_ptr<VertexArray> sq_vertex_array_;

    // --- static member data
    static Application* instance_;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Hazel
