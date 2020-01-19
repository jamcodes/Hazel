#include "Application.h"
#include "hzpch.h"

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

Application::Application() : window_{Window::Create()}
{
    window_->SetEventCallback([this](Event& e) { this->OnEvent(e); });
}
Application::~Application() = default;

void Application::pushLayer(std::unique_ptr<Layer> layer)
{
    layerStack_.pushLayer(std::move(layer));
}

void Application::pushOverlay(std::unique_ptr<Layer> overlay)
{
    layerStack_.pushOverlay(std::move(overlay));
}


void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->OnWindowClose(e); });

    HZ_CORE_TRACE("{0}", e);

    for(auto it{layerStack_.end()}; it != layerStack_.begin() && !e.handled; )
    {
        (--it)->get()->onEvent(e);
    }
}

void Application::Run()
{
    while (running_) {
        glClearColor(0, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& layer : layerStack_) {
            layer->onUpdate();
        }

        window_->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent&) noexcept
{
    running_ = false;
    return true;
}

}  // namespace Hazel
