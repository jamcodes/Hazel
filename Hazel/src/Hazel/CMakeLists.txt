cmake_minimum_required(VERSION 3.15)

target_sources(Hazel
    PRIVATE
        OrthographicCameraController.h
        OrthographicCameraController.cpp
)
add_subdirectory(Core)
add_subdirectory(Events)
add_subdirectory(ImGui)
add_subdirectory(Renderer)
