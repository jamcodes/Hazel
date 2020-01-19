cmake_minimum_required(VERSION 3.15)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KHR/khrplatform.h>
)
