cmake_minimum_required(VERSION 3.15)

add_subdirectory(Hazel)

target_sources(Hazel
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Hazel.h>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}/Hazel.h>
)

target_include_directories(Hazel
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    PRIVATE
)
