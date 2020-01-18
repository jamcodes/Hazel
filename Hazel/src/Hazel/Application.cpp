#include "Application.h"
#include "hzpch.h"

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

Application::Application() : window_{Window::Create()}
{
    window_->SetEventCallback([this](Event& e) { this->OnEvent(e); });
}
Application::~Application() = default;

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher{e};
    // dispatcher.Dispatch<WindowCloseEvent>([this](auto& e) { return this->OnWindowClose(e); });
    dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->OnWindowClose(e); });

    HZ_CORE_TRACE("{0}", e);
}

void Application::Run()
{
    while (running_) {
        glClearColor(0, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        window_->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent&) noexcept
{
    running_ = false;
    return true;
}

}  // namespace Hazel
