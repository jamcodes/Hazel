#pragma once

#include <chrono>

#include "Hazel.h"


namespace Sandbox
{

struct ProfileResult
{
    const char* name;
    std::chrono::microseconds duration;
};


class Sandbox2D : public Hazel::Layer {
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float time_delta_seconds) override;
    void onImGuiRender() override;
    void onEvent(Hazel::Event&) override;

private:
    Hazel::OrtographicCameraController camera_controller_;

    // temporary
    // Hazel::Ref<Hazel::VertexArray> sq_vertex_array_;
    // Hazel::Ref<Hazel::OpenGLShader> flat_color_shader_;

    Hazel::Ref<Hazel::Texture2D> checkerboard_texture_;

    glm::vec4 sq_color_{0.8f, 0.2f, 0.3f, 1.0f};
    glm::vec4 rect_color_{0.2f, 0.3f, 0.8f, 1.0f};

    std::vector<ProfileResult> profile_results_{};
};

} // namespace Sandbox
