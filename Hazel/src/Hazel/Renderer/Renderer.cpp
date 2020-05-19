#include "Renderer.h"

#include "Hazel/Renderer/Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

Renderer::SceneData* Renderer::s_scene_data_{new Renderer::SceneData{}};

void Renderer::init()
{
    HZ_PROFILE_FUNCTION();
    RenderCommand::init();
    Renderer2D::init();
}

void Renderer::onWindowResize(unsigned width, unsigned height) noexcept
{
    RenderCommand::setViewport(0, 0, width, height);
}

void Renderer::beginScene(OrtographicCamera const& camera)
{
    s_scene_data_->view_projection = camera.getViewProjection();
}

void Renderer::endScene() {}

void Renderer::submit(Shader const& /* shader */, VertexArray const& /* vertex_array */,
                      const glm::mat4& /* transform */)
{
    HZ_EXPECTS(false, DefaultCoreHandler, Hazel::Enforce, "Generic Renderer::submit not yet supported");
}

template<>
void Renderer::submit<OpenGLShader>(OpenGLShader const& shader, VertexArray const& vertex_array,
                      const glm::mat4& transform)
{
    shader.bind();
    shader.uploadUniform("u_view_projection", s_scene_data_->view_projection);
    shader.uploadUniform("u_transform", transform);

    vertex_array.bind();
    RenderCommand::drawIndexed(vertex_array);
}

}  // namespace Hazel
