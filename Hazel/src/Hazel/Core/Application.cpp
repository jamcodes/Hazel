#include "Application.h"

#include <GLFW/glfw3.h>

#include <chrono>

#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Shader.h"
#include "hzpch.h"

namespace {

inline void poll_keys_status(std::chrono::milliseconds interval)
{
    using namespace Hazel;
    static auto last_report_time{std::chrono::steady_clock::now()};
    if (std::chrono::steady_clock().now() - last_report_time >= interval) {
        auto const lmb_status{Hazel::Input::isMouseButtonPressed(MouseButton::Left)};
        auto const rmb_status{Hazel::Input::isMouseButtonPressed(MouseButton::Right)};
        auto const [mouse_x, mouse_y]{Hazel::Input::getMousePosition()};
        auto const ctrl_status{Hazel::Input::isKeyPressed(KeyCode::Left_control)};
        auto const alt_status{Hazel::Input::isKeyPressed(KeyCode::Left_alt)};
        auto const shift_status{Hazel::Input::isKeyPressed(KeyCode::Left_shift)};
        HZ_CORE_INFO("LMB: {0}, RMB: {1}, Pos: {{{2},{3}}}", lmb_status, rmb_status, mouse_x, mouse_y);
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

Application::Application() : window_{Window::create()}
{
    HZ_CORE_INFO("CONTRACT_LEVEL_CONFIG = {}", HZ_CONTRACT_LEVEL_CONFIG);
    // TODO: Make this a sane singleton
    HZ_PROFILE_FUNCTION();
    HZ_EXPECTS(Application::instance_ == nullptr, ApplicationAssertionHandler, Hazel::Enforce,
               "Hazel::Application already instantiated");
    Application::instance_ = this;
    window_->setEventCallback([this](Event& e) { this->onEvent(e); });

    Renderer::init();

    auto imgui_layer{std::make_unique<ImGuiLayer>()};
    imgui_layer_ = imgui_layer.get();
    pushOverlay(std::move(imgui_layer));
}
Application::~Application() = default;

void Application::pushLayer(std::unique_ptr<Layer> layer)
{
    HZ_PROFILE_FUNCTION();
    auto& layer_ref{*layer};
    layerStack_.pushLayer(std::move(layer));
    layer_ref.onAttach();
}

void Application::pushOverlay(std::unique_ptr<Layer> layer)
{
    HZ_PROFILE_FUNCTION();
    auto& layer_ref{*layer};
    layerStack_.pushOverlay(std::move(layer));
    layer_ref.onAttach();
}

void Application::run()
{
    while (running_) {
        HZ_PROFILE_SCOPE("Application::run() loop");
        auto const time_delta{last_frame_time_.tick()};

        if (not minimized_) {
            HZ_PROFILE_SCOPE("Application::run() -> layers update");
            for (auto& layer : layerStack_) {
                layer->onUpdate(Timestep::asFloat(time_delta));
            }
        }

        // TODO: "Preaction - postaction" style code, refactor the begin/end
        // Introduce a scope guard?
        imgui_layer_->begin();
        {
            HZ_PROFILE_SCOPE("Application::run() -> layers ImGuiRender");
            for (auto& layer : layerStack_) {
                layer->onImGuiRender();
            }
        }
        imgui_layer_->end();

        poll_keys_status(std::chrono::milliseconds{500});
        window_->onUpdate();
    }
}

void Application::close() noexcept
{
    running_ = false;
}

void Application::onEvent(Event& e)
{
    HZ_PROFILE_FUNCTION();
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->onWindowClose(e); });
    dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return this->onWindowResize(e); });

    for (auto it{layerStack_.end()}; it != layerStack_.begin() && !e.handled;) {
        if (e.handled) { break; }
        (*--it)->onEvent(e);
    }
}

bool Application::onWindowClose(WindowCloseEvent&) noexcept
{
    running_ = false;
    return true;
}

bool Application::onWindowResize(WindowResizeEvent& e) noexcept
{
    HZ_PROFILE_FUNCTION();
    auto const width{e.getWidth()};
    auto const height{e.getHeight()};
    if (width == 0 || height == 0) {
        minimized_ = true;
        return false;
    }

    minimized_ = false;
    Renderer::onWindowResize(width, height);
    return false;
}

}  // namespace Hazel
