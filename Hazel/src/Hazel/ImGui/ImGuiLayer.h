#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel {
class HAZEL_API ImGuiLayer : public Hazel::Layer {
private:
    float time_{0.0f};

public:
    ImGuiLayer();
    ~ImGuiLayer() override;

    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onEvent(Event&) override;

private:
    bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    bool onMouseMovedEvent(MouseMovedEvent& e);
    bool onMouseScrolledEvent(MouseScrolledEvent& e);
    bool onKeyPressedEvent(KeyPressedEvent& e);
    bool onKeyReleasedEvent(KeyReleasedEvent& e);
    bool onKeyTypedEvent(KeyTypedEvent& e);
    bool onWindowResizedEvent(WindowResizeEvent& e);
};

}  // namespace Hazel
