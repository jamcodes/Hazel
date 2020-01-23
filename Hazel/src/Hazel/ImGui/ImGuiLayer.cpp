#include "ImGuiLayer.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Hazel/Application.h"
#include "Platform/OpenGL/ImGuiOpenGlRenderer.h"

namespace Hazel {

ImGuiLayer::ImGuiLayer() : Layer{"ImGuiLayer"} {}
ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::onAttach()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io{ImGui::GetIO()};
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // TODO: Replace with Hazel key codes
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiLayer::onDetach() {}

void ImGuiLayer::onUpdate()
{
    auto& io{ImGui::GetIO()};
    Application& app{Application::get()};
    io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

    auto const time = static_cast<float>(glfwGetTime());
    io.DeltaTime = time_ > 0.0f ? (time - time_) : (1.0f / 60.0f);
    time_ = time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show{true};
    ImGui::ShowDemoWindow(&show);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::onEvent(Event& e)
{
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<MouseButtonPressedEvent>(
        [this](auto& e) { return this->onMouseButtonPressedEvent(e); });
    dispatcher.dispatch<MouseButtonReleasedEvent>(
        [this](auto& e) { return this->onMouseButtonReleasedEvent(e); });
    dispatcher.dispatch<MouseMovedEvent>(
        [this](auto& e) { return this->onMouseMovedEvent(e); });
    dispatcher.dispatch<MouseScrolledEvent>(
        [this](auto& e) { return this->onMouseScrolledEvent(e); });
    dispatcher.dispatch<KeyPressedEvent>([this](auto& e) { return this->onKeyPressedEvent(e); });
    dispatcher.dispatch<KeyReleasedEvent>([this](auto& e) { return this->onKeyReleasedEvent(e); });
    // dispatcher.dispatch<KeyTypedEvent>([this](auto& e) { return this->onKeyTypedEvent(e); });
    dispatcher.dispatch<WindowResizeEvent>([this](auto& e) { return this->onWindowResizeEvent(e); });
}

bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
    ImGuiIO& io{ImGui::GetIO()};
    io.MouseDown[e.getMouseButton()] = true;
    return false;
}

bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
    ImGuiIO& io{ImGui::GetIO()};
    return false;
}

bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& e)
{
    ImGuiIO& io{ImGui::GetIO()};
    return false;
}

bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& e)
{
    ImGuiIO& io{ImGui::GetIO()};
    return false;
}

bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& e)
{
    ImGuiIO& io{ImGui::GetIO()};
    return false;
}

bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& e)
{
    ImGuiIO& io{ImGui::GetIO()};
    return false;
}

// bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e)
// {
//     ImGuiIO& io{ImGui::GetIO()};
//     return false;
// }

bool ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& e)
{
    ImGuiIO& io{ImGui::GetIO()};
    return false;
}

}  // namespace Hazel
