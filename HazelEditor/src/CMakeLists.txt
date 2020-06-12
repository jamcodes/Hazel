cmake_minimum_required(VERSION 3.15)

target_sources(HazelEditor
    PRIVATE
        EditorLayer.h
        EditorLayer.cpp
        HazelEditor.cpp
)
target_include_directories(HazelEditor
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
