cmake_minimum_required(VERSION 3.15)
project(imgui LANGUAGES CXX)

add_library(${PROJECT_NAME} OBJECT)
target_sources(${PROJECT_NAME}
    PRIVATE
        imgui/imgui.h
        imgui/imconfig.h
        imgui/imgui_internal.h
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_definitions(${PROJECT_NAME}
    # Hacky solution to compiling Hazel with ImGui as a shared library
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>: "IMGUI_API=__declspec(dllexport)" >
        $<$<BOOL:${BUILD_SHARED_LIBS}>: "IMGUI_IMPL_API=__declspec(dllexport)" >
)
