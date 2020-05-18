#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <chrono>
#include <glm/gtc/type_ptr.hpp>

namespace Sandbox {

template <typename Function>
class Timer {
public:
    explicit Timer(const char* name, Function&& func) noexcept
        : name_{name}, cbk_{std::move(func)}, start_{std::chrono::steady_clock::now()}
    {
    }
    Timer(Timer const&) noexcept = default;
    Timer(Timer&&) noexcept = default;
    Timer& operator=(Timer const&) noexcept = default;
    Timer& operator=(Timer&&) noexcept = default;
    ~Timer() noexcept
    {
        if (!stopped_) {
            stop();
        }
    }

    void stop()
    {
        auto const end{std::chrono::steady_clock::now()};
        auto const duration{end - start_};
        stopped_ = true;
        cbk_(name_, std::chrono::duration_cast<std::chrono::microseconds>(duration));
    }

private:
    const char* name_;
    Function cbk_;
    bool stopped_{false};
    std::chrono::time_point<std::chrono::steady_clock> start_;
};

Sandbox2D::Sandbox2D() : Layer{"Sandbox2D"}, camera_controller_{1280.0f / 720.0f, true} {}

void Sandbox2D::onAttach()
{
    HZ_PROFILE_FUNCTION();
    checkerboard_texture_ = Hazel::Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() { HZ_PROFILE_FUNCTION(); }

void Sandbox2D::onUpdate(float time_delta_seconds)
{
    HZ_PROFILE_FUNCTION();
    camera_controller_.onUpdate(time_delta_seconds);

    {
        HZ_PROFILE_SCOPE("CameraController::onUpdate");
        Hazel::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::clear();
    }

    HZ_PROFILE_SCOPE("CameraController::onUpdate");
    Hazel::Renderer2D::beginScene(camera_controller_.getCamera());
    Hazel::Renderer2D::drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, sq_color_);
    Hazel::Renderer2D::drawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, rect_color_);
    Hazel::Renderer2D::drawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, *checkerboard_texture_);
    Hazel::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(sq_color_));
    ImGui::ColorEdit4("Rectangle Color", glm::value_ptr(rect_color_));
    ImGui::End();
}

void Sandbox2D::onEvent(Hazel::Event& e) { camera_controller_.onEvent(e); }

}  // namespace Sandbox
