#include "Application.h"
#include "hzpch.h"

#include "Hazel/AssertionHandler.h"
#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"
#include "Hazel/Log.h"
#include "Hazel/MouseButtonCodes.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Shader.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <chrono>
#include <string_view>

namespace {
const std::string vertex_src(R"(
    #version 450 core
    
    layout(location = 0) in vec3 a_position;

    out vec3 v_position;

    void main()
    {
        v_position = a_position;
        gl_Position = vec4(a_position, 1.0);
    }
)");

const std::string fragment_src(R"(
    #version 450 core
    
    layout(location = 0) out vec4 color;

    in vec3 v_position;

    void main()
    {
        color = vec4(v_position * 0.5 + 0.3, 1.0);
    }
)");
}  // namespace

namespace Hazel {

struct ApplicationAssertionHandler : Hazel::CoreLoggingHandler, Hazel::Enforce {
};

template <typename T, std::size_t N>
constexpr auto array_sizeof(T const (&)[N]) noexcept
{
    return sizeof(T) * N;
}

template <typename T, std::size_t N>
constexpr auto array_sizeof(std::array<T, N> const&) noexcept
{
    return sizeof(T) * N;
}

Application* Application::instance_{nullptr};

Application::Application() : window_{Window::create()}
{
    // TODO: Make this a sane singleton
    HZ_EXPECT(Application::instance_ == nullptr, ApplicationAssertionHandler{},
              "Hazel::Application already instantiated");
    Application::instance_ = this;
    window_->setEventCallback([this](Event& e) { this->onEvent(e); });
    auto imgui_layer{std::make_unique<ImGuiLayer>()};
    imgui_layer_ = imgui_layer.get();
    pushOverlay(std::move(imgui_layer));
    initGLData();
    shader_.reset(new Shader{vertex_src, fragment_src});
}
Application::~Application() = default;

bool Application::onWindowClose(WindowCloseEvent&) noexcept
{
    running_ = false;
    return true;
}

void Application::initGLData() noexcept
{
    constexpr std::array<float, 3 * 3> vertices{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                                            0.0f,  0.0f,  0.5f, 0.0f};

    vertex_buffer_ =
        VertexBuffer::create(vertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    constexpr std::array<unsigned int, 3> indices{0, 1, 2};
    index_buffer_ = IndexBuffer::create(indices);
}

void Application::pushLayer(std::unique_ptr<Layer> layer)
{
    layerStack_.pushLayer(std::move(layer));
}

void Application::pushOverlay(std::unique_ptr<Layer> layer)
{
    layerStack_.pushOverlay(std::move(layer));
}

void Application::onEvent(Event& e)
{
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<WindowCloseEvent>(
        [this](WindowCloseEvent& e) { return this->onWindowClose(e); });

    // HZ_CORE_TRACE("{0}", e);

    for (auto it{layerStack_.end()}; it != layerStack_.begin() && !e.handled;) {
        (--it)->get()->onEvent(e);
    }
}

namespace {
inline void poll_keys_status(std::chrono::milliseconds interval)
{
    static auto last_report_time{std::chrono::steady_clock::now()};
    if (std::chrono::steady_clock().now() - last_report_time >= interval) {
        auto const lmb_status{Input::isMouseButtonPressed(toInteger<int>(MouseButton::Left))};
        auto const rmb_status{Input::isMouseButtonPressed(toInteger<int>(MouseButton::Right))};
        auto const [mouse_x, mouse_y]{Input::getMousePosition()};
        auto const ctrl_status{Input::isKeyPressed(toInteger<int>(KeyCode::Left_control))};
        auto const alt_status{Input::isKeyPressed(toInteger<int>(KeyCode::Left_alt))};
        auto const shift_status{Input::isKeyPressed(toInteger<int>(KeyCode::Left_shift))};
        HZ_CORE_INFO("LMB: {0}, RMB: {1}, Pos: {{{2},{3}}}", lmb_status, rmb_status, mouse_x,
                     mouse_y);
        HZ_CORE_INFO("CTRL: {0}, ALT: {1}, SHIFT: {2}", ctrl_status, alt_status, shift_status);
        last_report_time = std::chrono::steady_clock::now();
    }
}
}  // namespace

void Application::run()
{
    while (running_) {
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_->bind();
        glBindVertexArray(vertex_array_);
        glDrawElements(GL_TRIANGLES, index_buffer_->getCount(), GL_UNSIGNED_INT, nullptr);

        for (auto& layer : layerStack_) {
            layer->onUpdate();
        }

        // TODO: "Preaction - postaction" style code, refactor the begin/end
        // it should probably be called from within onImGuiRender
        imgui_layer_->begin();
        for (auto& layer : layerStack_) {
            layer->onImGuiRender();
        }
        imgui_layer_->end();

        poll_keys_status(std::chrono::milliseconds{500});
        window_->onUpdate();
    }
}

}  // namespace Hazel
