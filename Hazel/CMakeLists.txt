cmake_minimum_required(VERSION 3.15)

project(Hazel VERSION 0.1.0 LANGUAGES CXX)
DeclareProjectInstallDirectories()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED OpenGL)


add_library(Hazel)
add_library(Hazel::Hazel ALIAS Hazel)

add_subdirectory(vendor)
add_subdirectory(src)

target_link_libraries(Hazel
    PUBLIC
        spdlog::spdlog
        imgui
        $<$<BOOL:${OPENGL_gl_LIBRARY}>:OpenGL::GL>
        $<$<NOT:$<BOOL:${OPENGL_gl_LIBRARY}>>:OpenGL::OpenGL>
        $<$<BOOL:${OPENGL_GLU_FOUND}>:OpenGL::GLU>
        $<$<BOOL:${OpenGL_GLX_FOUND}>:OpenGL::GLX>
        $<$<BOOL:${OpenGL_EGL_FOUND}>:OpenGL::EGL>
        glfw
        glad
        glm::glm
    PRIVATE
        HzBuildFlags
)

target_compile_definitions(Hazel
    PUBLIC
        HZ_PLATFORM_$<UPPER_CASE:$<PLATFORM_ID>>
        # Configure GLFW not to include OpenGL headers since we want glad to do it
        GLFW_INCLUDE_NONE
        $<$<CONFIG:Debug>:HZ_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:HZ_RELEASE>
        $<$<CONFIG:Release>:HZ_DIST>
        $<$<BOOL:${BUILD_SHARED_LIBS}>:HZ_DYNAMIC_LINK>
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:HZ_BUILD_DLL>
    # Hacky solution to compiling Hazel with ImGui as a shared library
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>: "IMGUI_API=__declspec(dllimport)" >
        $<$<BOOL:${BUILD_SHARED_LIBS}>: "IMGUI_IMPL_API=__declspec(dllimport)" >
)

set_target_properties(Hazel
    PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION}
        MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<BOOL:${BUILD_SHARED_LIBS}>:DLL>
        RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Hazel
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/Hazel
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}/Hazel
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/Hazel
        ARCHIVE_OUTPUT_DIRECTORY                ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Hazel
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/Hazel
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO}/Hazel
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}/Hazel
        LIBRARY_OUTPUT_DIRECTORY                ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Hazel
        LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/Hazel
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO}/Hazel
        LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/Hazel
)

set_source_files_properties(
    src/Hazel/ImGui/ImGuiBuild.cpp
    PROPERTIES
        COMPILE_OPTIONS "/wd4996;"  # Argument given to COMPILE_OPTIONS needs to be a list
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/
    DESTINATION ${Hazel_install_include_dir}
    FILES_MATCHING PATTERN "*.h"
)

InstallProject(TARGETS Hazel imgui glfw glad glm HzBuildFlags)
