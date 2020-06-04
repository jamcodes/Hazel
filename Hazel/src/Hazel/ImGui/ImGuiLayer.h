#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {
class HAZEL_API ImGuiLayer : public Hazel::Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() override;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float /* time_delta */) override { }
    void onImGuiRender() override;
    void onEvent(Event&) noexcept override;

    void begin();
    void end();

private:
    float time_{0.0f};
};

}  // namespace Hazel
