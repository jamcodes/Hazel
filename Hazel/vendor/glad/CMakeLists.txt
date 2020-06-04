cmake_minimum_required(VERSION 3.15)
project(glad)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED OpenGL)

add_library(${PROJECT_NAME} OBJECT)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        $<IF:$<BOOL:${OPENGL_gl_LIBRARY}>,OpenGL::GL,OpenGL::OpenGL>
        # $<$<BOOL:${OPENGL_gl_LIBRARY}>:OpenGL::GL>
        # $<$<NOT:$<BOOL:${OPENGL_gl_LIBRARY}>>:OpenGL::OpenGL>
        $<$<BOOL:${OPENGL_GLU_FOUND}>:OpenGL::GLU>
        $<$<BOOL:${OpenGL_GLX_FOUND}>:OpenGL::GLX>
        $<$<BOOL:${OpenGL_EGL_FOUND}>:OpenGL::EGL>
)
