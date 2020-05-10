#include "Sandbox2D.h"

#include <imgui/imgui.h>

// #include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Sandbox {

Sandbox2D::Sandbox2D()
    : Layer{"Sandbox2D"}
    , camera_controller_{1280.0f/720.0f, true}
{
}

void Sandbox2D::onAttach()
{
    sq_vertex_array_ = Hazel::VertexArray::create();
    // clang-format off
    constexpr std::array<float, 3 * 4> vertices{
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    // clang-format on
    const Hazel::BufferLayout layout = {
        {Hazel::ShaderDataType::Float3, "a_position"},
    };
    sq_vertex_array_->addVertexBuffer(Hazel::VertexBuffer::create(vertices, layout));

    constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
    sq_vertex_array_->setIndexBuffer(Hazel::IndexBuffer::create(sq_indices));

    flat_color_shader_ =
        Hazel::Shader::create<Hazel::OpenGLShader>("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::onDetach() {}

void Sandbox2D::onUpdate(float time_delta_seconds)
{
    // HZ_TRACE("time_delta_seconds = {}", time_delta_seconds);
    camera_controller_.onUpdate(time_delta_seconds);

    Hazel::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
    Hazel::RenderCommand::clear();

    Hazel::Renderer::beginScene(camera_controller_.getCamera());

    const glm::mat4 scale{glm::scale(glm::mat4{1.0f}, glm::vec3{0.1f})};

    flat_color_shader_->bind();
    flat_color_shader_->uploadUniform("u_color", sq_color_);

    Hazel::Renderer::submit(*flat_color_shader_, *sq_vertex_array_,
                            glm::scale(glm::mat4{1.0f}, glm::vec3{1.5f}));

    Hazel::Renderer::endScene();
}

void Sandbox2D::onImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(sq_color_));
    ImGui::End();
}

void Sandbox2D::onEvent(Hazel::Event& e) { camera_controller_.onEvent(e); }

}  // namespace Sandbox
