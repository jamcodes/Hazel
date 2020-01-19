#include "Application.h"
#include "hzpch.h"

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

struct ApplicationAssertionHandler : Hazel::CoreLoggingHandler, Hazel::Enforce {
};

Application* Application::instance_{nullptr};

Application::Application() : window_{Window::create()}
{
    // TODO: Make this a sane singleton
    HZ_EXPECT(Application::instance_ == nullptr, ApplicationAssertionHandler{}, "Hazel::Application already instantiated");
    Application::instance_ = this;
    window_->setEventCallback([this](Event& e) { this->onEvent(e); });
}
Application::~Application() = default;

void Application::pushLayer(std::unique_ptr<Layer> layer)
{
    layer->onAttach();
    layerStack_.pushLayer(std::move(layer));
}

void Application::pushOverlay(std::unique_ptr<Layer> layer)
{
    layer->onAttach();
    layerStack_.pushOverlay(std::move(layer));
}


void Application::onEvent(Event& e)
{
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->onWindowClose(e); });

    HZ_CORE_TRACE("{0}", e);

    for(auto it{layerStack_.end()}; it != layerStack_.begin() && !e.handled; )
    {
        (--it)->get()->onEvent(e);
    }
}

void Application::run()
{
    while (running_) {
        glClearColor(0, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& layer : layerStack_) {
            layer->onUpdate();
        }

        window_->onUpdate();
    }
}

bool Application::onWindowClose(WindowCloseEvent&) noexcept
{
    running_ = false;
    return true;
}

}  // namespace Hazel
