cmake_minimum_required(VERSION 3.15)
project(imgui LANGUAGES CXX)

add_library(${PROJECT_NAME} OBJECT)
target_sources(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp>
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
