#pragma once

#include <glm/glm.hpp>

#include "OrtographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Hazel {

class Renderer {
public:
    static void beginScene(OrtographicCamera const&);
    static void endScene();
    static void submit(Shader const& shader, VertexArray const& vertexArray);
    static inline RendererAPI::API getApi() noexcept { return RendererAPI::getAPI(); }

private:
    struct SceneData{
        glm::mat4 view_projection;
    };

    static SceneData* s_scene_data_;
};

}  // namespace Hazel
