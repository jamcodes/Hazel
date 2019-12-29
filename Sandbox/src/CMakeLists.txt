cmake_minimum_required(VERSION 3.15)

target_sources(Sandbox
    PRIVATE
        SandboxApp.cpp
)
target_include_directories(Sandbox
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
