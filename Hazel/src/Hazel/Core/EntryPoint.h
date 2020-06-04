#pragma once
#include "Hazel/Core/Base.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int /* argc */, char* /* argv */[])
{
    Hazel::Log::Init();
    int a = 5;

    const auto shutdown = [](Hazel::Application* p) {
        HZ_PROFILE_END_SESSION();   // End the Runtime session;
        delete p;
        HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
        HZ_PROFILE_END_SESSION();   // end the shutdown session
    };
    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
    auto app = std::unique_ptr<Hazel::Application, decltype(shutdown)>{Hazel::CreateApplication(), shutdown};
    HZ_PROFILE_END_SESSION();
    HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->run();
}

#endif
