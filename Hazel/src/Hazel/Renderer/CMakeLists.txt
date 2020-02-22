cmake_minimum_required(VERSION 3.15)

target_sources(${PROJECT_NAME}
    PRIVATE
        Buffer.cpp
        GraphicsContext.h
        OrtographicCamera.cpp
        OrtographicCamera.h
        RenderCommand.cpp
        RenderCommand.h
        Renderer.cpp
        Renderer.h
        RendererAPI.cpp
        RendererAPI.h
        Shader.cpp
        Shader.h
        VertexArray.cpp
        VertexArray.h
        Buffer.h
        Texture.h
        Texture.cpp
)
