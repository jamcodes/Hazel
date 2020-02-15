#include "Application.h"
#include "hzpch.h"

#include "Hazel/AssertionHandler.h"
#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"
#include "Hazel/Log.h"
#include "Hazel/MouseButtonCodes.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Shader.h"

#include <GLFW/glfw3.h>

#include <chrono>

namespace {
const std::string vertex_src(R"(
    #version 450 core
    
    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec4 a_color;

    uniform mat4 u_view_projection;

    out vec3 v_position;
    out vec4 v_color;

    void main()
    {
        v_position = a_position;
        v_color = a_color;
        gl_Position = u_view_projection * vec4(a_position, 1.0);
    }
)");

const std::string fragment_src(R"(
    #version 450 core
    
    layout(location = 0) out vec4 color;

    in vec3 v_position;
    in vec4 v_color;

    void main()
    {
        color = vec4(v_position * 0.5 + 0.3, 1.0);
        color = v_color;
    }
)");

const std::string blue_vertex_src(R"(
    #version 450 core
    
    layout(location = 0) in vec3 a_position;

    uniform mat4 u_view_projection;

    out vec3 v_position;

    void main()
    {
        v_position = a_position;
        gl_Position = u_view_projection * vec4(a_position, 1.0);
    }
)");

const std::string blue_fragment_src(R"(
    #version 450 core
    
    layout(location = 0) out vec4 color;

    in vec3 v_position;

    void main()
    {
        color = vec4(0.2, 0.3, 0.8, 1.0);
    }
)");

glm::vec3 camera_pan_move{0.0f, 0.0f, 0.0f};

inline glm::vec3 getCameraMove(Hazel::KeyCode const code) noexcept
{
    auto move{camera_pan_move};
    switch (code) {
        case Hazel::KeyCode::W:{
            move.y = 0.02f;
            break;
            }
        case Hazel::KeyCode::A:
            move.x = -0.02f;
            break;
        case Hazel::KeyCode::D:
            move.x = 0.02f;
            break;
        case Hazel::KeyCode::S:
            move.y = -0.02f;
            break;
        default:
            break;
        }
        return move;
}

inline void handleCameraPan(Hazel::OrtographicCamera& camera,
                            Hazel::KeyPressedEvent const event) noexcept
{
    auto const move{getCameraMove(Hazel::KeyCode{event.getKeyCode()})};
    auto const& current_position{camera.getPosition()};
    camera.setPosition(current_position + move);
    if (event.getRepeatCount() != 0) {
        camera_pan_move = glm::clamp(move, -0.02f, 0.02f);
    }
}

inline void handleCameraPan(Hazel::KeyReleasedEvent const event) noexcept
{
    // auto const move{getCameraMove(Hazel::KeyCode{event.getKeyCode()})};
    // camera_pan_move = glm::clamp(camera_pan_move - move, -0.2f, 0.2f);
        switch (Hazel::KeyCode code{event.getKeyCode()}; code) {
        case Hazel::KeyCode::W:{
            camera_pan_move.y -= 0.02f;
            break;
            }
        case Hazel::KeyCode::A:
            camera_pan_move.x -= -0.02f;
            break;
        case Hazel::KeyCode::D:
            camera_pan_move.x -= 0.02f;
            break;
        case Hazel::KeyCode::S:
            camera_pan_move.y -= -0.02f;
            break;
        default:
            break;
        }
        camera_pan_move = glm::clamp(camera_pan_move, -0.02f, 0.02f);
}

inline void handleCameraRotate(Hazel::OrtographicCamera& camera, Hazel::KeyCode const code) noexcept
{
    switch (auto const& current_rotation{camera.getRotation()}; code) {
    case Hazel::KeyCode::D: {
        camera.setRotation(current_rotation + 1.0f);
        return;
    }
    case Hazel::KeyCode::A: {
        camera.setRotation(current_rotation - 1.0f);
        return;
    }
    case Hazel::KeyCode::S: {
        camera.setRotation(current_rotation - 1.0f);
        return;
    }
    case Hazel::KeyCode::W: {
        camera.setRotation(current_rotation + 1.0f);
        return;
    }
    default:
        return;
    }
    return;
}

inline void poll_keys_status(std::chrono::milliseconds interval)
{
    using namespace Hazel;
    static auto last_report_time{std::chrono::steady_clock::now()};
    if (std::chrono::steady_clock().now() - last_report_time >= interval) {
        auto const lmb_status{
            Hazel::Input::isMouseButtonPressed(toInteger<int>(MouseButton::Left))};
        auto const rmb_status{
            Hazel::Input::isMouseButtonPressed(toInteger<int>(MouseButton::Right))};
        auto const [mouse_x, mouse_y]{Hazel::Input::getMousePosition()};
        auto const ctrl_status{Hazel::Input::isKeyPressed(toInteger<int>(KeyCode::Left_control))};
        auto const alt_status{Hazel::Input::isKeyPressed(toInteger<int>(KeyCode::Left_alt))};
        auto const shift_status{Hazel::Input::isKeyPressed(toInteger<int>(KeyCode::Left_shift))};
        HZ_CORE_INFO("LMB: {0}, RMB: {1}, Pos: {{{2},{3}}}", lmb_status, rmb_status, mouse_x,
                     mouse_y);
        HZ_CORE_INFO("CTRL: {0}, ALT: {1}, SHIFT: {2}", ctrl_status, alt_status, shift_status);
        last_report_time = std::chrono::steady_clock::now();
    }
}

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

Application::Application() : window_{Window::create()}, camera_{-1.6f, 1.6f, -0.9f, 0.9f}
{
    // TODO: Make this a sane singleton
    HZ_EXPECT(Application::instance_ == nullptr, ApplicationAssertionHandler{}, Hazel::Enforce{},
              "Hazel::Application already instantiated");
    Application::instance_ = this;
    window_->setEventCallback([this](Event& e) { this->onEvent(e); });
    auto imgui_layer{std::make_unique<ImGuiLayer>()};
    imgui_layer_ = imgui_layer.get();
    pushOverlay(std::move(imgui_layer));
    initGLData();
}
Application::~Application() = default;

bool Application::onWindowClose(WindowCloseEvent&) noexcept
{
    running_ = false;
    return true;
}

void Application::initGLData() noexcept
{
    shader_.reset(new Shader{vertex_src, fragment_src});
    tr_vertex_array_ = VertexArray::create();
    constexpr std::array<float, 3 * 7> tr_vertices{-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                                                   0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                                                   0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f};

    const BufferLayout tr_layout = {
        {ShaderDataType::Float3, "a_position"},
        {ShaderDataType::Float4, "a_color"},
    };

    auto tr_vertex_buffer = VertexBuffer::create(tr_vertices, tr_layout);
    tr_vertex_array_->addVertexBuffer(std::move(tr_vertex_buffer));

    constexpr std::array<unsigned int, 3> tr_indices{0, 1, 2};
    auto tr_index_buffer = IndexBuffer::create(tr_indices);
    tr_vertex_array_->setIndexBuffer(std::move(tr_index_buffer));

    shader_sq_.reset(new Shader{blue_vertex_src, blue_fragment_src});
    sq_vertex_array_ = VertexArray::create();
    constexpr std::array<float, 3 * 4> sq_vertices{
        -0.75f, -0.75f, 0.0f, 0.75f, -0.75f, 0.0f, 0.75f, 0.75f, 0.0f, -0.75f, 0.75f, 0.0f,
    };
    const BufferLayout sq_layout = {
        {ShaderDataType::Float3, "a_position"},
    };
    sq_vertex_array_->addVertexBuffer(VertexBuffer::create(sq_vertices, sq_layout));

    constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
    sq_vertex_array_->setIndexBuffer(IndexBuffer::create(sq_indices));
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
    dispatcher.dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) {
        if (not Input::isKeyPressed(toInteger<int>(KeyCode::Left_control))) {
            handleCameraPan(this->camera_, e);
        }
        else {
            handleCameraRotate(camera_, KeyCode{e.getKeyCode()});
        }
        return false;  // don't consume the event
    });
    dispatcher.dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& e) {
        handleCameraPan(e);
        return false;
    });

    for (auto it{layerStack_.end()}; it != layerStack_.begin() && !e.handled;) {
        (--it)->get()->onEvent(e);
    }
}

void Application::run()
{
    while (running_) {
        RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::clear();

        Renderer::beginScene(camera_);

        Renderer::submit(*shader_sq_, *sq_vertex_array_);
        Renderer::submit(*shader_, *tr_vertex_array_);

        Renderer::endScene();

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
