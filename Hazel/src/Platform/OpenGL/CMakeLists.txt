cmake_minimum_required(VERSION 3.15)

target_sources(${PROJECT_NAME}
    PRIVATE
        OpenGLContext.h
        OpenGLContext.cpp
        OpenGLBuffer.h
        OpenGLBuffer.cpp
        OpenGLVertexArray.h
        OpenGLVertexArray.cpp
        OpenGLRendererAPI.h
        OpenGLRendererAPI.cpp
        OpenGLShader.h
        OpenGLShader.cpp
        OpenGLTexture.h
        OpenGLTexture.cpp
        OpenGLFramebuffer.h
        OpenGLFramebuffer.cpp
)
