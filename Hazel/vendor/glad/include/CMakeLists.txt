cmake_minimum_required(VERSION 3.15)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${Hazel_install_include_dir}>
)

target_sources(${PROJECT_NAME}
    PRIVATE
        glad/glad.h
        KHR/khrplatform.h
)
