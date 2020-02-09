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
    layout(location = 1) in vec4 a_color;

    out vec3 v_position;
    out vec4 v_color;

    void main()
    {
        v_position = a_position;
        v_color = a_color;
        gl_Position = vec4(a_position, 1.0);
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

template <>
constexpr GLenum shaderDataTypeToPlatformType<GLenum>(ShaderDataType type) noexcept {
    // clang-format off
    switch(type) {
        case ShaderDataType::None  :    return GL_NONE;
        case ShaderDataType::Float :    return GL_FLOAT;
        case ShaderDataType::Float2:    return GL_FLOAT;
        case ShaderDataType::Float3:    return GL_FLOAT;
        case ShaderDataType::Float4:    return GL_FLOAT;
        case ShaderDataType::Mat3  :    return GL_FLOAT;
        case ShaderDataType::Mat4  :    return GL_FLOAT;
        case ShaderDataType::Int   :    return GL_INT;
        case ShaderDataType::Int2  :    return GL_INT;
        case ShaderDataType::Int3  :    return GL_INT;
        case ShaderDataType::Int4  :    return GL_INT;
        case ShaderDataType::Bool  :    return GL_BOOL;
    }
    // clang-format on
    HZ_ASSERT(false, DefaultCoreHandler, Enforce, "Unknown ShaderDataType");
    return GL_NONE;
}

Application* Application::instance_{nullptr};

Application::Application() : window_{Window::create()}
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
    constexpr std::array<float, 3 * 7> vertices {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };

    const BufferLayout layout = {
        {ShaderDataType::Float3, "a_position"},
        {ShaderDataType::Float4, "a_color"},
    };

    vertex_buffer_ = VertexBuffer::create(vertices, layout);

    auto const& vb_layout{vertex_buffer_->getLayout()};
    for (std::uint32_t i{0}; i != vb_layout.getElements().size(); ++i) {
        auto const& el{vb_layout.getElements()[i]};
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            componentCount(el.type),
            shaderDataTypeToPlatformType<GLenum>(el.type),
            el.normalized ? GL_TRUE : GL_FALSE,
            vb_layout.getStride(),
            reinterpret_cast<const void*>(el.offset));
    }

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
