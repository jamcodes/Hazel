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
    Ref<Shader> texture_shader;    // Used for both textures and flat colors
    Ref<Texture2D> white_texture;  // used to eliminate the texture component when using the shader as a flat-color
};

void Renderer2D::init()
{
    HZ_PROFILE_FUNCTION();
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
    const BufferLayout layout = {{ShaderDataType::Float3, "a_position"}, {ShaderDataType::Float2, "a_tex_coord"}};
    s_data->vertex_array->addVertexBuffer(VertexBuffer::create(vertices, layout));

    constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
    s_data->vertex_array->setIndexBuffer(IndexBuffer::create(sq_indices));

    s_data->white_texture = Texture2D::create(1, 1);
    const unsigned white_texture_data{0xffffffff};
    s_data->white_texture->setData(&white_texture_data, sizeof(white_texture_data));

    s_data->texture_shader = Shader::create("assets/shaders/Texture.glsl");
    s_data->texture_shader->bind();
    s_data->texture_shader->setUniform("u_texture", 0);
}

void Renderer2D::shutdown()
{
    HZ_PROFILE_FUNCTION();
    delete s_data;
}

void Renderer2D::beginScene(const OrtographicCamera& camera)
{
    HZ_PROFILE_FUNCTION();
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
    HZ_PROFILE_FUNCTION();
    auto& shader{*s_data->texture_shader};
    // shader.bind();   // not necessary to rebind on every draw if there's only one shader
    shader.setUniform("u_color", color);
    shader.setUniform("u_tiling_factor", 1.0f);
    // bind the white texture here - the white texture will result in a uniform value of 1.0 in the shader
    // meaning that the `u_texture` component of the shader draw expression essentially has no effect
    s_data->white_texture->bind();

    // translation * rotation * scale   (note - no rotation here)
    glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    shader.setUniform("u_transform", std::move(transform));

    auto& vxa{*s_data->vertex_array};
    vxa.bind();
    RenderCommand::drawIndexed(vxa);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture,
                          float tiling_factor, const glm::vec4& tint_color)
{
    drawQuad({position.x, position.y, 0.0f}, size, texture, tiling_factor, tint_color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Texture2D& texture,
                          float tiling_factor, const glm::vec4& tint_color)
{
    HZ_PROFILE_FUNCTION();
    auto& shader{*s_data->texture_shader};
    // shader.bind();   // not necessary to rebind on every draw if there's only one shader
    // set color to white on every draw - meaning that the `u_color` component of the shader draw expression
    // has no effect. Setting this on every draw is necessary, since the same `u_color` is used by the flat-color
    // drawQuad call
    shader.setUniform("u_color", tint_color);
    shader.setUniform("u_tiling_factor", tiling_factor);
    texture.bind();

    // translation * rotation * scale   (note - no rotation here)
    glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    shader.setUniform("u_transform", std::move(transform));

    auto& vxa{*s_data->vertex_array};
    vxa.bind();
    RenderCommand::drawIndexed(vxa);
}

void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation,
                                 const glm::vec4& color)
{
    drawQuadRotated({position.x, position.y, 0.0f}, size, rotation, color);
}

void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation,
                                 const glm::vec4& color)
{
    HZ_PROFILE_FUNCTION();
    auto& shader{*s_data->texture_shader};
    // shader.bind();   // not necessary to rebind on every draw if there's only one shader
    shader.setUniform("u_color", color);
    shader.setUniform("u_tiling_factor", 1.0f);
    // bind the white texture here - the white texture will result in a uniform value of 1.0 in the shader
    // meaning that the `u_texture` component of the shader draw expression essentially has no effect
    s_data->white_texture->bind();

    // translation * rotation * scale   (note - no rotation here)
    glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                        glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    shader.setUniform("u_transform", std::move(transform));

    auto& vxa{*s_data->vertex_array};
    vxa.bind();
    RenderCommand::drawIndexed(vxa);
}

void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation,
                                 const Texture2D& texture, float tiling_factor, const glm::vec4& tint_color)
{
    drawQuadRotated({position.x, position.y, 0.0f}, size, rotation, texture, tiling_factor, tint_color);
}

void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation,
                                 const Texture2D& texture, float tiling_factor, const glm::vec4& tint_color)
{
    HZ_PROFILE_FUNCTION();
    auto& shader{*s_data->texture_shader};
    // shader.bind();   // not necessary to rebind on every draw if there's only one shader
    // set color to white on every draw - meaning that the `u_color` component of the shader draw expression
    // has no effect. Setting this on every draw is necessary, since the same `u_color` is used by the flat-color
    // drawQuad call
    shader.setUniform("u_color", tint_color);
    shader.setUniform("u_tiling_factor", tiling_factor);
    texture.bind();

    // translation * rotation * scale   (note - no rotation here)
    glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                        glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    shader.setUniform("u_transform", std::move(transform));

    auto& vxa{*s_data->vertex_array};
    vxa.bind();
    RenderCommand::drawIndexed(vxa);
}

}  // namespace Hazel
