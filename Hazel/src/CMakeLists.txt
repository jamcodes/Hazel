cmake_minimum_required(VERSION 3.15)

add_subdirectory(Hazel)
add_subdirectory(Platform)

target_sources(Hazel
    PRIVATE
        Hazel.h
)
target_include_directories(Hazel
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}>
)

target_precompile_headers(Hazel
    PUBLIC
        $<BUILD_INTERFACE:hzpch.h>
)
