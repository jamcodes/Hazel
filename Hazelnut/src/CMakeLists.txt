cmake_minimum_required(VERSION 3.15)

target_sources(Hazelnut
    PRIVATE
        EditorLayer.h
        EditorLayer.cpp
        Hazelnut.cpp
)
target_include_directories(Hazelnut
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
