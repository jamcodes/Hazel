#pragma once

#include <glm/glm.hpp>

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Hazel {

class Renderer {
public:
    static void init();

    static void onWindowResize(unsigned width, unsigned height) noexcept;

    static void beginScene(OrthographicCamera const&);
    static void endScene();

    static void submit(Shader const& shader, VertexArray const& vertexArray,
                       const glm::mat4& transform = glm::mat4{1.0f});
    static inline RendererAPI::API getApi() noexcept { return RendererAPI::getAPI(); }

    template <typename ShaderT>
    static void submit(ShaderT const& shader, VertexArray const& vertexArray,
                       const glm::mat4& transform = glm::mat4{1.0f});

private:
    struct SceneData {
        glm::mat4 view_projection;
    };

    static SceneData* s_scene_data_;
};

}  // namespace Hazel
