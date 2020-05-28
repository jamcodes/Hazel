cmake_minimum_required(VERSION 3.15)

target_sources(${PROJECT_NAME}
    PRIVATE
        Buffer.cpp
        GraphicsContext.h
        OrthographicCamera.cpp
        OrthographicCamera.h
        RenderCommand.cpp
        RenderCommand.h
        Renderer.cpp
        Renderer.h
        Renderer2D.h
        Renderer2D.cpp
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
