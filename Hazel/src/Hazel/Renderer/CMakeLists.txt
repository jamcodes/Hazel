cmake_minimum_required(VERSION 3.15)

target_sources(${PROJECT_NAME}
    PRIVATE
        GraphicsContext.h
        Shader.h
        Shader.cpp
        Buffer.h
        Buffer.cpp
        Renderer.h
        Renderer.cpp
        VertexArray.h
        VertexArray.cpp
)
