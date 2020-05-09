cmake_minimum_required(VERSION 3.15)

target_sources(Hazel
    PRIVATE
        Application.cpp
        Application.h
        Assertion.h
        AssertionHandler.h
        Core.h
        EntryPoint.h
        EnumOperators.h
        KeyCodes.h
        Layer.cpp
        Layer.h
        LayerStack.cpp
        LayerStack.h
        Log.cpp
        Log.h
        MouseButtonCodes.h
        Window.h
        Timestep.h
)