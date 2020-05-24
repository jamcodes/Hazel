#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

struct QuadVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    // TODO: texture_id
};

struct Renderer2DData {
    static constexpr const std::uint32_t max_quads{10'000};
    static constexpr const std::uint32_t max_vertices{max_quads * 4};
    static constexpr const std::uint32_t max_indices{max_quads * 6};
    inline static std::array<QuadVertex, max_vertices> quad_vertex_buffer_array;

    Scope<VertexArray> quad_vertex_array;
    Ref<Shader> texture_shader;    // Used for both textures and flat colors
    Ref<Texture2D> white_texture;  // used to eliminate the texture component when using the shader as a flat-color
    std::uint32_t quad_index_count{0};
    QuadVertex* quad_vertex_buffer_ptr{nullptr};
    // std::array<QuadVertex, max_vertices>::iterator quad_vertex_buffer_it{quad_vertex_buffer_array.end()};
};
}  // namespace Hazel

namespace {
Hazel::Renderer2DData s_data;
}  // namespace

// constexpr const std::uint32_t Renderer2DData::max_quads;
// constexpr const std::uint32_t Renderer2DData::max_vertices;
// constexpr const std::uint32_t Renderer2DData::max_indices;
// std::array<QuadVertex, max_vertices> Renderer2DData::quad_vertex_buffer_array;

namespace Hazel {
void Renderer2D::init()
{
    HZ_PROFILE_FUNCTION();
    s_data.quad_vertex_array = VertexArray::create();
    // // clang-format off
    // constexpr std::array<float, 5 * 4> vertices{
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    //      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //      0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    //     -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    // };
    // clang-format on
    // const BufferLayout layout = {
    //     {ShaderDataType::Float3, "a_position"},
    //     {ShaderDataType::Float4, "a_color"},
    //     {ShaderDataType::Float2, "a_tex_coord"}
    // };
    // s_data.quad_vertex_array->addVertexBuffer(VertexBuffer::create(vertices, layout));
    auto quad_vertex_buffer = VertexBuffer::create(s_data.max_vertices * sizeof(QuadVertex));
    quad_vertex_buffer->setLayout({{ShaderDataType::Float3, "a_position"},
                                   {ShaderDataType::Float4, "a_color"},
                                   {ShaderDataType::Float2, "a_tex_coord"}});
    s_data.quad_vertex_array->addVertexBuffer(std::move(quad_vertex_buffer));

    constexpr std::array<std::uint32_t, Renderer2DData::max_indices> quad_indices = []() constexpr {
        std::array<std::uint32_t, Renderer2DData::max_indices> indices{};

        std::uint32_t offset{0};
        for(std::uint32_t i{0}; i < s_data.max_indices; i += 6, offset += 4) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
        }

        return indices;
    }();
    // constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
    s_data.quad_vertex_array->setIndexBuffer(IndexBuffer::create(quad_indices));

    s_data.white_texture = Texture2D::create(1, 1);
    const unsigned white_texture_data{0xffffffff};
    s_data.white_texture->setData(&white_texture_data, sizeof(white_texture_data));

    s_data.texture_shader = Shader::create("assets/shaders/Texture.glsl");
    s_data.texture_shader->bind();
    s_data.texture_shader->setUniform("u_texture", 0);
}

void Renderer2D::shutdown() { HZ_PROFILE_FUNCTION(); }

void Renderer2D::beginScene(const OrtographicCamera& camera)
{
    HZ_PROFILE_FUNCTION();
    auto& ts{*s_data.texture_shader};
    ts.bind();
    ts.setUniform("u_view_projection", camera.getViewProjection());

    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_array.data();
}

void Renderer2D::endScene()
{
    HZ_PROFILE_FUNCTION();
    auto const data_size{
        static_cast<std::uint32_t>((s_data.quad_vertex_buffer_ptr - s_data.quad_vertex_buffer_array.data()) *
                                   static_cast<std::uint32_t>(sizeof(s_data.quad_vertex_buffer_array.front())))};
    s_data.quad_vertex_array->getVertexBuffers().back()->setData(s_data.quad_vertex_buffer_array.data(), data_size);
    // s_data.quad_vertex_buffer->setData();
    flush();
}

void Renderer2D::flush() { RenderCommand::drawIndexed(*s_data.quad_vertex_array, s_data.quad_index_count); }

// primitives
void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    HZ_PROFILE_FUNCTION();

    s_data.quad_vertex_buffer_ptr->position = position;
    s_data.quad_vertex_buffer_ptr->color = color;
    s_data.quad_vertex_buffer_ptr->tex_coord = {0.0f, 0.0f};
    ++s_data.quad_vertex_buffer_ptr;

    s_data.quad_vertex_buffer_ptr->position = {position.x + size.x, position.y, 0.0f};
    s_data.quad_vertex_buffer_ptr->color = color;
    s_data.quad_vertex_buffer_ptr->tex_coord = {1.0f, 0.0f};
    ++s_data.quad_vertex_buffer_ptr;

    s_data.quad_vertex_buffer_ptr->position = {position.x + size.x, position.y + size.y, 0.0f};
    s_data.quad_vertex_buffer_ptr->color = color;
    s_data.quad_vertex_buffer_ptr->tex_coord = {1.0f, 1.0f};
    ++s_data.quad_vertex_buffer_ptr;

    s_data.quad_vertex_buffer_ptr->position = {position.x, position.y + size.y, 0.0f};
    s_data.quad_vertex_buffer_ptr->color = color;
    s_data.quad_vertex_buffer_ptr->tex_coord = {0.0f, 1.0f};
    ++s_data.quad_vertex_buffer_ptr;

    s_data.quad_index_count += 6;  // why +6?

    // HZ_ASSERT(s_data.quad_vertex_buffer_ptr <= (s_data.quad_vertex_buffer_array.data() +
    // ::Hazel::Renderer2DData::max_vertices),
    //           "Batch Vertex buffer array overflow");

    // auto& shader{*s_data.texture_shader};
    // shader.bind();   // not necessary to rebind on every draw if there's only one shader
    // shader.setUniform("u_color", color);
    // shader.setUniform("u_tiling_factor", 1.0f);
    // bind the white texture here - the white texture will result in a uniform value of 1.0 in the shader
    // meaning that the `u_texture` component of the shader draw expression essentially has no effect
    // s_data.white_texture->bind();

    // translation * rotation * scale   (note - no rotation here)
    // glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
    //                     glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    // shader.setUniform("u_transform", std::move(transform));

    // auto& vxa{*s_data.quad_vertex_array};
    // vxa.bind();
    // RenderCommand::drawIndexed(vxa);
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
    auto& shader{*s_data.texture_shader};
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

    auto& vxa{*s_data.quad_vertex_array};
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
    auto& shader{*s_data.texture_shader};
    // shader.bind();   // not necessary to rebind on every draw if there's only one shader
    shader.setUniform("u_color", color);
    shader.setUniform("u_tiling_factor", 1.0f);
    // bind the white texture here - the white texture will result in a uniform value of 1.0 in the shader
    // meaning that the `u_texture` component of the shader draw expression essentially has no effect
    s_data.white_texture->bind();

    // translation * rotation * scale   (note - no rotation here)
    glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                        glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};
    shader.setUniform("u_transform", std::move(transform));

    auto& vxa{*s_data.quad_vertex_array};
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
    auto& shader{*s_data.texture_shader};
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

    auto& vxa{*s_data.quad_vertex_array};
    vxa.bind();
    RenderCommand::drawIndexed(vxa);
}

}  // namespace Hazel
