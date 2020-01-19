cmake_minimum_required(VERSION 3.15)

target_sources(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glad.c>
)
