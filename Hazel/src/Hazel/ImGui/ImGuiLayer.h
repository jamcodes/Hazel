#pragma once

#include "Hazel/Layer.h"

namespace Hazel
{
class HAZEL_API ImGuiLayer : public Hazel::Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() override;

    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onEvent(Event&) override;

private:
    float time_{0.0f};
};
} // namespace Hazel
