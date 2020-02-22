cmake_minimum_required(VERSION 3.15)
project(stb)

add_library(${PROJECT_NAME} OBJECT)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
        stb/stb_image.h
        stb/stb_image.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
