cmake_minimum_required(VERSION 3.15)

project(HazelEngine)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(options)
include(config)
include(release)

SetGlobalFlags(DEFAULT_BUILD_TYPE Debug)
DeclareCompilerConfigTarget(HzBuildFlags)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(Hazel)
add_subdirectory(Sandbox)
