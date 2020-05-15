#include "Sandbox2D.h"

#include <imgui/imgui.h>

// #include <glm/gtc/matrix_transform.hpp>
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

#define HZ_CONCATENATE_IMPL(s1, s2) s1##s2
#define HZ_CONCATENATE(s1, s2) HZ_CONCATENATE_IMPL(s1, s2)

#define HZ_PROFILE_SCOPE(nAME)                                                                                       \
    Timer HZ_CONCATENATE(timer, __LINE__){nAME, [this](auto&& name, auto&& duration) noexcept {                                      \
                              profile_results_.push_back(ProfileResult{std::forward<decltype(name)>(name),           \
                                                                       std::forward<decltype(duration)>(duration)}); \
                          }}

Sandbox2D::Sandbox2D() : Layer{"Sandbox2D"}, camera_controller_{1280.0f / 720.0f, true}
{
    profile_results_.reserve(42);
}

void Sandbox2D::onAttach()
{
    // sq_vertex_array_ = Hazel::VertexArray::create();
    // // clang-format off
    // constexpr std::array<float, 3 * 4> vertices{
    //     -0.5f, -0.5f, 0.0f,
    //      0.5f, -0.5f, 0.0f,
    //      0.5f,  0.5f, 0.0f,
    //     -0.5f,  0.5f, 0.0f
    // };
    // // clang-format on
    // const Hazel::BufferLayout layout = {
    //     {Hazel::ShaderDataType::Float3, "a_position"},
    // };
    // sq_vertex_array_->addVertexBuffer(Hazel::VertexBuffer::create(vertices, layout));

    // constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
    // sq_vertex_array_->setIndexBuffer(Hazel::IndexBuffer::create(sq_indices));

    // flat_color_shader_ =
    //     Hazel::Shader::create<Hazel::OpenGLShader>("assets/shaders/FlatColor.glsl");
    checkerboard_texture_ = Hazel::Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() {}

void Sandbox2D::onUpdate(float time_delta_seconds)
{
    HZ_PROFILE_SCOPE("Sandbox2D::onUpdate");
    // HZ_TRACE("time_delta_seconds = {}", time_delta_seconds);
    {
        HZ_PROFILE_SCOPE("CameraController::onUpdate");
        camera_controller_.onUpdate(time_delta_seconds);
    }
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

    for (auto const& result : profile_results_) {
        std::array<char, 50> label;
        std::strcpy(label.data(), result.name);
        std::strcat(label.data(), "  %d us");
        ImGui::Text(label.data(), result.duration.count());
    }
    profile_results_.clear();

    ImGui::End();
}

void Sandbox2D::onEvent(Hazel::Event& e) { camera_controller_.onEvent(e); }

}  // namespace Sandbox
