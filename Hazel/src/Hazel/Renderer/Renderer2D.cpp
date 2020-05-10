#include "Renderer2D.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/RenderCommand.h"

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
};

void Renderer2D::init()
{
    s_data = new Renderer2DStorage{};
    s_data->vertex_array = VertexArray::create();
    // clang-format off
    constexpr std::array<float, 3 * 4> vertices{
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    // clang-format on
    const BufferLayout layout = {
        {ShaderDataType::Float3, "a_position"},
    };
    s_data->vertex_array->addVertexBuffer(VertexBuffer::create(vertices, layout));

    constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
    s_data->vertex_array->setIndexBuffer(IndexBuffer::create(sq_indices));

    s_data->flat_color_shader = Shader::create("assets/shaders/FlatColor.glsl");
}

void Renderer2D::shutdown() { delete s_data; }

void Renderer2D::beginScene(const OrtographicCamera& camera)
{
    // TODO: get rid of the dynamic_cast here
    auto& gl_shader{*dynamic_cast<OpenGLShader*>(s_data->flat_color_shader.get())};
    gl_shader.bind();
    gl_shader.uploadUniform("u_view_projection", camera.getViewProjection());
    // TODO: remove fixed transform
    gl_shader.uploadUniform("u_transform", glm::mat4(1.0f));
}

void Renderer2D::endScene() {}

// primitives
void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    // TODO: get rid of the dynamic_cast here
    auto& gl_shader{*dynamic_cast<OpenGLShader*>(s_data->flat_color_shader.get())};
    gl_shader.bind();
    gl_shader.uploadUniform("u_color", color);
    s_data->vertex_array->bind();
    RenderCommand::drawIndexed(*s_data->vertex_array);
}

}  // namespace Hazel
