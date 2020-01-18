cmake_minimum_required(VERSION 3.15)

target_sources(Hazel
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Application.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Core.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/EntryPoint.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Log.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Window.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Layer.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LayerStack.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel/Application.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel/Core.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel/EntryPoint.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel/Log.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel/Window.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel/Layer.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel/LayerStack.h>
    PRIVATE
        Application.cpp
        Log.cpp
        Layer.cpp
        LayerStack.cpp
)
