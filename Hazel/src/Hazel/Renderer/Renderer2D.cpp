#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {
struct Renderer2DStorage;
};

namespace {
Hazel::Renderer2DStorage* s_data;
}  // namespace

namespace Hazel {

struct Renderer2DStorage {
    Scope<VertexArray> vertex_array;
    Scope<Shader> flat_color_shader;
    Ref<Shader> texture_shader;
};

void Renderer2D::init()
{
    s_data = new Renderer2DStorage{};
    s_data->vertex_array = VertexArray::create();
    // clang-format off
    constexpr std::array<float, 5 * 4> vertices{
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on
    const BufferLayout layout = {
        {ShaderDataType::Float3, "a_position"},
        {ShaderDataType::Float2, "a_tex_coord"}
    };
    s_data->vertex_array->addVertexBuffer(VertexBuffer::create(vertices, layout));

    constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
    s_data->vertex_array->setIndexBuffer(IndexBuffer::create(sq_indices));

    s_data->flat_color_shader = Shader::create("assets/shaders/FlatColor.glsl");
    s_data->texture_shader = Shader::create("assets/shaders/Texture.glsl");
    s_data->texture_shader->bind();
    s_data->texture_shader->setUniform("u_Texture", 0);
}

void Renderer2D::shutdown() { delete s_data; }

void Renderer2D::beginScene(const OrtographicCamera& camera)
{
    auto& fcs{*s_data->flat_color_shader};
    fcs.bind();
    fcs.setUniform("u_view_projection", camera.getViewProjection());

    auto& ts{*s_data->texture_shader};
    ts.bind();
    ts.setUniform("u_view_projection", camera.getViewProjection());
}

void Renderer2D::endScene() {}

// primitives
void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    auto& shader{*s_data->flat_color_shader};
    shader.bind();
    shader.setUniform("u_color", color);

    // translation * rotation * scale   (note - no rotation here)
    glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    shader.setUniform("u_transform", std::move(transform));

    auto& vxa{*s_data->vertex_array};
    vxa.bind();
    RenderCommand::drawIndexed(vxa);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture)
{
    drawQuad({position.x, position.y, 0.0f}, size, texture);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Texture2D& texture)
{
    auto& shader{*s_data->texture_shader};
    shader.bind();

    // translation * rotation * scale   (note - no rotation here)
    glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    shader.setUniform("u_transform", std::move(transform));

    texture.bind();

    auto& vxa{*s_data->vertex_array};
    vxa.bind();
    RenderCommand::drawIndexed(vxa);
}

}  // namespace Hazel
