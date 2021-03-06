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
    float tex_index;
    float tiling_factor;
};

struct Renderer2DData {
    static constexpr const std::uint32_t max_quads{10'000};
    static constexpr const std::uint32_t quad_vertex_count{4};
    static constexpr const std::uint32_t max_vertices{max_quads * quad_vertex_count};
    static constexpr const std::uint32_t max_indices{max_quads * 6};
    static constexpr const std::uint32_t max_texture_slots{32};  // TODO: Renderer-capabilities
    static constexpr const std::uint32_t first_texture_index{1};
    static constexpr const std::uint32_t white_texture_index{0};

    Scope<VertexArray> quad_vertex_array;
    Ref<Shader> texture_shader;    // Used for both textures and flat colors
    Ref<Texture2D> white_texture;  // used to eliminate the texture component when using the shader as a flat-color

    std::uint32_t quad_index_count{0};
    std::array<QuadVertex, max_vertices> quad_vertex_buffer_array;
    QuadVertex* quad_vertex_buffer_ptr{nullptr};
    std::array<glm::vec4, 4> quad_vertex_positions;

    std::array<Ref<Texture2D>, max_texture_slots> texture_slots;
    std::uint32_t texture_slot_index{first_texture_index};  // 0 == white texture

    Renderer2D::Statistics stats;
};
}  // namespace Hazel

namespace {
::Hazel::Renderer2DData s_data;

inline void increment_quad_index() noexcept
{
    s_data.quad_index_count += 6;  // why +6?
}
}  // namespace

namespace Hazel {
void Renderer2D::init()
{
    HZ_PROFILE_FUNCTION();
    s_data.quad_vertex_array = VertexArray::create();
    auto quad_vertex_buffer = VertexBuffer::create(s_data.max_vertices * sizeof(QuadVertex));
    quad_vertex_buffer->setLayout({{ShaderDataType::Float3, "a_position"},
                                   {ShaderDataType::Float4, "a_color"},
                                   {ShaderDataType::Float2, "a_tex_coord"},
                                   {ShaderDataType::Float, "a_tex_index"},
                                   {ShaderDataType::Float, "a_tiling_factor"}});
    s_data.quad_vertex_array->addVertexBuffer(std::move(quad_vertex_buffer));

    constexpr auto quad_indices = []() constexpr
    {
        std::array<std::uint32_t, Renderer2DData::max_indices> indices{};

        std::uint32_t offset{0};
        for (std::uint32_t i{0}; i < s_data.max_indices; i += 6, offset += 4) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
        }

        return indices;
    }
    ();

    s_data.quad_vertex_array->setIndexBuffer(IndexBuffer::create(quad_indices));

    s_data.white_texture = Texture2D::create(1, 1);
    const unsigned white_texture_data{0xffffffff};
    s_data.white_texture->setData(&white_texture_data, sizeof(white_texture_data));

    constexpr auto tex_samplers = []() constexpr
    {
        std::array<int, Renderer2DData::max_texture_slots> samplers{};
        for (auto i{0}; i != samplers.size(); ++i) samplers[i] = i;
        return samplers;
    }
    ();

    s_data.texture_shader = Shader::create("assets/shaders/Texture.glsl");
    s_data.texture_shader->bind();
    s_data.texture_shader->setUniform("u_textures", tex_samplers.data(),
                                      static_cast<std::uint32_t>(tex_samplers.size()));

    s_data.quad_vertex_positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    s_data.quad_vertex_positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    s_data.quad_vertex_positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    s_data.quad_vertex_positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::shutdown() { HZ_PROFILE_FUNCTION(); }

inline void Renderer2D::resetDrawBuffers() noexcept
{
    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_array.data();

    s_data.texture_slot_index = s_data.first_texture_index;
    s_data.texture_slots[s_data.white_texture_index] = s_data.white_texture;
}

void Renderer2D::beginScene(const OrthographicCamera& camera)
{
    HZ_PROFILE_FUNCTION();
    auto& ts{*s_data.texture_shader};
    ts.bind();
    ts.setUniform("u_view_projection", camera.getViewProjection());

    resetDrawBuffers();
}

inline void Renderer2D::flush()
{
    if (s_data.quad_index_count != 0) {
        // Bind textures
        for (std::uint32_t i{0}; i != s_data.texture_slot_index; ++i) {
            s_data.texture_slots[i]->bind(i);
        }
        RenderCommand::drawIndexed(*s_data.quad_vertex_array, s_data.quad_index_count);
        ++s_data.stats.draw_calls;
    }
}

void Renderer2D::endScene()
{
    HZ_PROFILE_FUNCTION();

    auto const data_size{
        static_cast<std::uint32_t>((s_data.quad_vertex_buffer_ptr - s_data.quad_vertex_buffer_array.data()) *
                                   sizeof(s_data.quad_vertex_buffer_array.front()))};
    s_data.quad_vertex_array->getVertexBuffers().back()->setData(s_data.quad_vertex_buffer_array.data(), data_size);

    flush();
}

inline void Renderer2D::checkAndFlush() noexcept
{
    if (s_data.quad_index_count >= Renderer2DData::max_indices) {
        endScene();
        resetDrawBuffers();
    }
}

// primitives
void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    HZ_PROFILE_FUNCTION();

    checkAndFlush();

    const glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};

    auto const fill_buffer = [&transform, &color](glm::vec4 const& pos, glm::vec2 tx_crd) noexcept {
        constexpr float tiling_factor{1.0f};

        s_data.quad_vertex_buffer_ptr->position = transform * pos;
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->tex_coord = tx_crd;
        s_data.quad_vertex_buffer_ptr->tex_index = s_data.white_texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        ++s_data.quad_vertex_buffer_ptr;
    };

    fill_buffer(s_data.quad_vertex_positions[0], {0.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[1], {1.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[2], {1.0f, 1.0f});
    fill_buffer(s_data.quad_vertex_positions[3], {0.0f, 1.0f});

    increment_quad_index();
    ++s_data.stats.quad_count;
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                          float tiling_factor, const glm::vec4& tint_color)
{
    drawQuad({position.x, position.y, 0.0f}, size, texture, tiling_factor, tint_color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                          float tiling_factor, const glm::vec4& tint_color)
{
    HZ_PROFILE_FUNCTION();

    checkAndFlush();

    auto const texture_index = [&texture]() noexcept {
        for (std::uint32_t i{0}; i != s_data.texture_slot_index; ++i) {
            if (*s_data.texture_slots[i] == *texture) {
                return static_cast<float>(i);
            }
        }
        return static_cast<float>(s_data.texture_slot_index);
    }();

    // If texture not found
    if (static_cast<const std::uint32_t>(texture_index) == s_data.texture_slot_index) {
        if (s_data.texture_slot_index >= Renderer2DData::max_texture_slots) {
            checkAndFlush();
        }
        HZ_ASSERT(s_data.texture_slot_index != s_data.max_texture_slots, "Maximum texture slots exceeded");
        s_data.texture_slots[static_cast<const std::uint32_t>(texture_index)] = texture;
        ++s_data.texture_slot_index;
    }

    const glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};

    auto const fill_buffer = [&transform, &tint_color, texture_index, tiling_factor](glm::vec4 const& pos,
                                                                                     glm::vec2 tx_crd) noexcept {
        s_data.quad_vertex_buffer_ptr->position = transform * pos;
        s_data.quad_vertex_buffer_ptr->color = tint_color;
        s_data.quad_vertex_buffer_ptr->tex_coord = tx_crd;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        ++s_data.quad_vertex_buffer_ptr;
    };
    fill_buffer(s_data.quad_vertex_positions[0], {0.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[1], {1.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[2], {1.0f, 1.0f});
    fill_buffer(s_data.quad_vertex_positions[3], {0.0f, 1.0f});

    increment_quad_index();
    ++s_data.stats.quad_count;
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture,
                          float tiling_factor, const glm::vec4& tint_color)
{
    drawQuad({position.x, position.y, 0.0f}, size, subtexture, tiling_factor, tint_color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture,
                          float tiling_factor, const glm::vec4& tint_color)
{
    HZ_PROFILE_FUNCTION();

    checkAndFlush();

    auto const& tex_coords{subtexture->getCoords()};
    auto const& texture{subtexture->getTexture()};

    auto const texture_index = [&texture]() noexcept {
        for (std::uint32_t i{0}; i != s_data.texture_slot_index; ++i) {
            if (*s_data.texture_slots[i] == *texture) {
                return static_cast<float>(i);
            }
        }
        return static_cast<float>(s_data.texture_slot_index);
    }();
    // If texture not found
    if (static_cast<const std::uint32_t>(texture_index) == s_data.texture_slot_index) {
        if (s_data.texture_slot_index >= Renderer2DData::max_texture_slots) {
            checkAndFlush();
        }
        HZ_ASSERT(s_data.texture_slot_index != s_data.max_texture_slots, "Maximum texture slots exceeded");
        s_data.texture_slots[static_cast<const std::uint32_t>(texture_index)] = texture;
        ++s_data.texture_slot_index;
    }

    const glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};

    auto const fill_buffer = [&transform, &tint_color, texture_index, tiling_factor](glm::vec4 const& pos,
                                                                                     glm::vec2 tx_crd) noexcept {
        s_data.quad_vertex_buffer_ptr->position = transform * pos;
        s_data.quad_vertex_buffer_ptr->color = tint_color;
        s_data.quad_vertex_buffer_ptr->tex_coord = tx_crd;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        ++s_data.quad_vertex_buffer_ptr;
    };

    for (std::uint32_t i{0}; i != Renderer2DData::quad_vertex_count; ++i) {
        fill_buffer(s_data.quad_vertex_positions[i], tex_coords[i]);
    }

    increment_quad_index();
    ++s_data.stats.quad_count;
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

    checkAndFlush();

    const glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                              glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};

    auto const fill_buffer = [&transform, &color](glm::vec4 const& pos, glm::vec2 tx_crd) noexcept {
        constexpr float tiling_factor{1.0f};
        s_data.quad_vertex_buffer_ptr->position = transform * pos;
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->tex_coord = tx_crd;
        s_data.quad_vertex_buffer_ptr->tex_index = s_data.white_texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        ++s_data.quad_vertex_buffer_ptr;
    };
    fill_buffer(s_data.quad_vertex_positions[0], {0.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[1], {1.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[2], {1.0f, 1.0f});
    fill_buffer(s_data.quad_vertex_positions[3], {0.0f, 1.0f});

    increment_quad_index();
    ++s_data.stats.quad_count;
}

void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation,
                                 const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
{
    drawQuadRotated({position.x, position.y, 0.0f}, size, rotation, texture, tiling_factor, tint_color);
}

void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation,
                                 const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
{
    HZ_PROFILE_FUNCTION();

    checkAndFlush();

    auto const texture_index = [&texture]() noexcept {
        for (std::uint32_t i{0}; i != s_data.texture_slot_index; ++i) {
            if (*s_data.texture_slots[i] == *texture) {
                return static_cast<float>(i);
            }
        }
        return static_cast<float>(s_data.texture_slot_index);
    }();
    // If texture not found
    if (static_cast<const std::uint32_t>(texture_index) == s_data.texture_slot_index) {
        HZ_ASSERT(s_data.texture_slot_index != s_data.max_texture_slots, "Maximum texture slots exceeded");
        s_data.texture_slots[static_cast<const std::uint32_t>(texture_index)] = texture;
        ++s_data.texture_slot_index;
    }

    const glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                              glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};

    auto const fill_buffer = [&transform, &tint_color, texture_index, tiling_factor](glm::vec4 const& pos,
                                                                                     glm::vec2 tx_crd) noexcept {
        s_data.quad_vertex_buffer_ptr->position = transform * pos;
        s_data.quad_vertex_buffer_ptr->color = tint_color;
        s_data.quad_vertex_buffer_ptr->tex_coord = tx_crd;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        ++s_data.quad_vertex_buffer_ptr;
    };
    fill_buffer(s_data.quad_vertex_positions[0], {0.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[1], {1.0f, 0.0f});
    fill_buffer(s_data.quad_vertex_positions[2], {1.0f, 1.0f});
    fill_buffer(s_data.quad_vertex_positions[3], {0.0f, 1.0f});

    increment_quad_index();
    ++s_data.stats.quad_count;
}

void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation,
                                 const Ref<SubTexture2D>& subtexture, float tiling_factor, const glm::vec4& tint_color)
{
    drawQuadRotated({position.x, position.y, 0.0f}, size, rotation, subtexture, tiling_factor, tint_color);
}

void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation,
                                 const Ref<SubTexture2D>& subtexture, float tiling_factor, const glm::vec4& tint_color)
{
    HZ_PROFILE_FUNCTION();

    checkAndFlush();

    auto const& tex_coords{subtexture->getCoords()};
    auto const& texture{subtexture->getTexture()};

    auto const texture_index = [&texture]() noexcept {
        for (std::uint32_t i{0}; i != s_data.texture_slot_index; ++i) {
            if (*s_data.texture_slots[i] == *texture) {
                return static_cast<float>(i);
            }
        }
        return static_cast<float>(s_data.texture_slot_index);
    }();
    // If texture not found
    if (static_cast<const std::uint32_t>(texture_index) == s_data.texture_slot_index) {
        HZ_ASSERT(s_data.texture_slot_index != s_data.max_texture_slots, "Maximum texture slots exceeded");
        s_data.texture_slots[static_cast<const std::uint32_t>(texture_index)] = texture;
        ++s_data.texture_slot_index;
    }

    const glm::mat4 transform{glm::translate(glm::mat4(1.0f), position) *
                              glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})};

    auto const fill_buffer = [&transform, &tint_color, texture_index, tiling_factor](glm::vec4 const& pos,
                                                                                     glm::vec2 tx_crd) noexcept {
        s_data.quad_vertex_buffer_ptr->position = transform * pos;
        s_data.quad_vertex_buffer_ptr->color = tint_color;
        s_data.quad_vertex_buffer_ptr->tex_coord = tx_crd;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        ++s_data.quad_vertex_buffer_ptr;
    };
    for (std::uint32_t i{0}; i != Renderer2DData::quad_vertex_count; ++i) {
        fill_buffer(s_data.quad_vertex_positions[i], tex_coords[i]);
    }

    increment_quad_index();
    ++s_data.stats.quad_count;
}

void Renderer2D::resetStats() noexcept { s_data.stats = Renderer2D::Statistics{}; }

Renderer2D::Statistics Renderer2D::getStats() noexcept { return s_data.stats; }

}  // namespace Hazel
