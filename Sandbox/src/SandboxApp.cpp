#include <Hazel.h>
#include <imgui/imgui.h>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer() : Layer{"ExampleLayer"} { }
    ~ExampleLayer() = default;
    ExampleLayer& operator=(ExampleLayer&&) = delete;

    void onAttach() override { }
    void onDetach() override { }

    void onUpdate() override
    {
        // HZ_TRACE("ExampleLayer::Update");
    }

    void onEvent(Hazel::Event& /* event */) override
    {
        // HZ_TRACE("ExampleLayer: {0}", event);
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello world", IMGUI_VERSION);
        ImGui::End();
    }
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        pushLayer(std::make_unique<ExampleLayer>());
    }
    ~Sandbox() = default;
    Sandbox& operator=(Sandbox&&) noexcept = delete;
};

Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}
