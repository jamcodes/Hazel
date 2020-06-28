cmake_minimum_required(VERSION 3.15)

target_sources(Hazel
    PRIVATE
        Application.cpp
        Application.h
        Assertion.h
        AssertionHandler.h
        Base.h
        EntryPoint.h
        EnumOperators.h
        Input.h
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
