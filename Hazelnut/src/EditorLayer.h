#pragma once

#include <chrono>

#include "Hazel.h"

namespace Hazel
{

class EditorLayer : public Layer {
public:
    EditorLayer();
    ~EditorLayer() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float time_delta_seconds) override;
    void onImGuiRender() override;
    void onEvent(Event&) override;

private:
    void drawTileMap();

    OrthographicCameraController camera_controller_;

    Ref<Texture2D> checkerboard_texture_;
    Ref<Texture2D> sprite_sheet_;
    Ref<SubTexture2D> texture_stairs_;
    Ref<Framebuffer> framebuffer_;
    std::vector<Ref<SubTexture2D>> sprites_;

    glm::vec4 sq_color_{0.8f, 0.2f, 0.3f, 1.0f};
    glm::vec4 rect_color_{0.2f, 0.3f, 0.8f, 1.0f};

    // FramebufferSpecification framebuffer_spec_;
    glm::vec2 viewport_size_{};
    // glm::vec2 new_viewport_size_{};
};

} // namespace Hazel
