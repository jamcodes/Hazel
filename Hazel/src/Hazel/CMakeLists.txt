cmake_minimum_required(VERSION 3.15)

target_sources(Hazel
    PRIVATE
        OrtographicCameraController.h
        OrtographicCameraController.cpp
)
add_subdirectory(Core)
add_subdirectory(Events)
add_subdirectory(ImGui)
add_subdirectory(Renderer)
