#include "Renderer.h"

namespace Hazel {

Renderer::SceneData* Renderer::s_scene_data_{new Renderer::SceneData{}};

void Renderer::beginScene(OrtographicCamera const& camera)
{
    s_scene_data_->view_projection = camera.getViewProjection();
}

void Renderer::endScene()
{

}

void Renderer::submit(Shader const& shader, VertexArray const& vertex_array)
{
    shader.bind();
    shader.uploadUniform("u_view_projection", s_scene_data_->view_projection);

    vertex_array.bind();
    RenderCommand::drawIndexed(vertex_array);
}


}  // namespace Hazel
