cmake_minimum_required(VERSION 3.15)

add_subdirectory(spdlog)

# Hardcode some GLFW options
# Backup the global BUILD_SHARED_LIBS
set(hazel_build_shared_libs ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS   OFF) # Force static linking
set(GLFW_BUILD_DOCS     OFF) # Don't build docs
set(GLFW_INSTALL        OFF) # Don't declare install targets
add_subdirectory(GLFW)
set(BUILD_SHARED_LIBS ${hazel_build_shared_libs})
unset(hazel_build_shared_libs)
target_link_libraries(glfw PRIVATE HzBuildFlags)

add_subdirectory(glad)
target_link_libraries(glad PRIVATE HzBuildFlags)

add_subdirectory(imgui)
target_link_libraries(imgui PRIVATE HzBuildFlags)

add_subdirectory(glm)
