#include <Hazel.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
const std::string vertex_src(R"(
    #version 450 core
    
    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec4 a_color;

    uniform mat4 u_view_projection;
    uniform mat4 u_transform;

    out vec3 v_position;
    out vec4 v_color;

    void main()
    {
        v_position = a_position;
        v_color = a_color;
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
    }
)");

const std::string fragment_src(R"(
    #version 450 core
    
    layout(location = 0) out vec4 color;

    in vec3 v_position;
    in vec4 v_color;

    void main()
    {
        color = vec4(v_position * 0.5 + 0.3, 1.0);
        color = v_color;
    }
)");

const std::string flat_color_vertex_src(R"(
    #version 450 core
    
    layout(location = 0) in vec3 a_position;

    uniform mat4 u_view_projection;
    uniform mat4 u_transform;

    out vec3 v_position;

    void main()
    {
        v_position = a_position;
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
    }
)");

const std::string flat_color_fragment_src(R"(
    #version 450 core
    
    layout(location = 0) out vec4 color;

    in vec3 v_position;

    uniform vec3 u_color;

    void main()
    {
        color = vec4(u_color, 1.0);
    }
)");

// const std::string texture_shader_vertex_src(R"(
//     #version 450 core

//     layout(location = 0) in vec3 a_position;
//     layout(location = 1) in vec2 a_tex_coord;

//     uniform mat4 u_view_projection;
//     uniform mat4 u_transform;

//     out vec2 v_tex_coord;

//     void main()
//     {
//         v_tex_coord = a_tex_coord;
//         gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
//     }
// )");

// const std::string texture_shader_fragment_src(R"(
//     #version 450 core
    
//     layout(location = 0) out vec4 color;

//     in vec2 v_tex_coord;

//     uniform sampler2D u_texture;

//     void main()
//     {
//         color = texture(u_texture, v_tex_coord);
//     }
// )");
}  // namespace

class ExampleLayer : public Hazel::Layer {
public:
    ExampleLayer() : Layer{"ExampleLayer"}, camera_{-1.6f, 1.6f, -0.9f, 0.9f} { initRenderer(); }

    ~ExampleLayer() = default;
    ExampleLayer& operator=(ExampleLayer&&) = delete;

    void onAttach() override {}
    void onDetach() override {}

    void onUpdate(float const time_delta_seconds) override
    {
        HZ_TRACE("time_delta_seconds = {}", time_delta_seconds);
        cameraMove(time_delta_seconds);

        Hazel::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::clear();

        Hazel::Renderer::beginScene(camera_);

        const glm::mat4 scale{glm::scale(glm::mat4{1.0f}, glm::vec3{0.1f})};

        sq_shader_->bind();
        sq_shader_->uploadUniform("u_color", sq_color_);

        for (auto y{0}; y != 20; ++y) {
            for (auto x{0}; x != 20; ++x) {
                glm::vec3 pos{-1.1f + x * 0.11f, -1.1f + y * 0.11f, 0.0f};
                glm::mat4 transform{glm::translate(glm::mat4{1.0f}, pos) * scale};
                Hazel::Renderer::submit(*sq_shader_, *sq_vertex_array_, transform);
            }
        }

        // Hazel::Renderer::submit(*sq_shader_, *sq_vertex_array_, glm::scale(glm::mat4{1.0f}, glm::vec3{1.5f}));
        texture_->bind();
        Hazel::Renderer::submit(*texture_shader_, *sq_vertex_array_, glm::scale(glm::mat4{1.0f}, glm::vec3{1.5f}));
        texture_cherno_logo_->bind();
        Hazel::Renderer::submit(*texture_shader_, *sq_vertex_array_, glm::scale(glm::mat4{1.0f}, glm::vec3{1.5f}));


        // Triangle
        // Hazel::Renderer::submit(*tr_shader_, *tr_vertex_array_);

        Hazel::Renderer::endScene();
    }

    void onEvent(Hazel::Event&) override {}

    void onImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(sq_color_));
        ImGui::End();
    }

private:
    void initRenderer() noexcept
    {
        tr_shader_ = Hazel::Shader::create<Hazel::OpenGLShader>(vertex_src, fragment_src);
        tr_vertex_array_ = Hazel::VertexArray::create();
        // clang-format off
        constexpr std::array<float, 3 * 7> tr_vertices{
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        // clang-format on
        const Hazel::BufferLayout tr_layout = {
            {Hazel::ShaderDataType::Float3, "a_position"},
            {Hazel::ShaderDataType::Float4, "a_color"},
        };

        auto tr_vertex_buffer = Hazel::VertexBuffer::create(tr_vertices, tr_layout);
        tr_vertex_array_->addVertexBuffer(std::move(tr_vertex_buffer));

        constexpr std::array<unsigned int, 3> tr_indices{0, 1, 2};
        auto tr_index_buffer = Hazel::IndexBuffer::create(tr_indices);
        tr_vertex_array_->setIndexBuffer(std::move(tr_index_buffer));

        sq_shader_ = Hazel::Shader::create<Hazel::OpenGLShader>(flat_color_vertex_src, flat_color_fragment_src);
        sq_vertex_array_ = Hazel::VertexArray::create();
        // clang-format off
        constexpr std::array<float, 5 * 4> sq_vertices{
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };
        // clang-format on
        const Hazel::BufferLayout sq_layout = {
            {Hazel::ShaderDataType::Float3, "a_position"},
            {Hazel::ShaderDataType::Float2, "a_tex_coord"},
        };
        sq_vertex_array_->addVertexBuffer(Hazel::VertexBuffer::create(sq_vertices, sq_layout));

        constexpr std::array<unsigned int, 6> sq_indices{0, 1, 2, 2, 3, 0};
        sq_vertex_array_->setIndexBuffer(Hazel::IndexBuffer::create(sq_indices));

        // texture_shader_ = Hazel::Shader::create<Hazel::OpenGLShader>(texture_shader_vertex_src, texture_shader_fragment_src);
        texture_shader_ = Hazel::Shader::create<Hazel::OpenGLShader>("assets/shaders/Texture.glsl");
        texture_ = Hazel::Texture2D::create("assets/textures/Checkerboard.png");
        texture_cherno_logo_ = Hazel::Texture2D::create("assets/textures/ChernoLogo.png");
        texture_shader_->bind();
        texture_shader_->uploadUniform("u_texture", 0);
    }

    void cameraMove(float const time_delta_seconds) noexcept
    {
        if (not Hazel::Input::isKeyPressed(Hazel::KeyCode::Left_control)) {
            if (Hazel::Input::isKeyPressed(Hazel::KeyCode::W)) {
                camera_position_.y += camera_move_speed_ * time_delta_seconds;
            }
            if (Hazel::Input::isKeyPressed(Hazel::KeyCode::S)) {
                camera_position_.y -= camera_move_speed_ * time_delta_seconds;
            }
            if (Hazel::Input::isKeyPressed(Hazel::KeyCode::A)) {
                camera_position_.x -= camera_move_speed_ * time_delta_seconds;
            }
            if (Hazel::Input::isKeyPressed(Hazel::KeyCode::D)) {
                camera_position_.x += camera_move_speed_ * time_delta_seconds;
            }
            camera_.setPosition(camera_position_);
        }
        else {
            if (Hazel::Input::isKeyPressed(Hazel::KeyCode::A)) {
                camera_rotation_ += camera_rotation_speed_ * time_delta_seconds;
            }
            else if (Hazel::Input::isKeyPressed(Hazel::KeyCode::D)) {
                camera_rotation_ -= camera_rotation_speed_ * time_delta_seconds;
            }
            camera_.setRotation(camera_rotation_);
        }
    }

    // Temporary - should be generic Hazel::Shader
    Hazel::Scope<Hazel::OpenGLShader> tr_shader_;
    Hazel::Scope<Hazel::VertexArray> tr_vertex_array_;

    // Temporary - should be generic Hazel::Shader
    Hazel::Scope<Hazel::OpenGLShader> sq_shader_;
    Hazel::Scope<Hazel::OpenGLShader> texture_shader_;
    Hazel::Scope<Hazel::VertexArray> sq_vertex_array_;
    glm::vec3 sq_color_{0.2f, 0.3f, 0.8f};

    Hazel::Ref<Hazel::Texture2D> texture_;
    Hazel::Ref<Hazel::Texture2D> texture_cherno_logo_;

    Hazel::OrtographicCamera camera_;

    glm::vec3 camera_position_{0.0f};
    float camera_rotation_{0.0f};

    static constexpr float camera_move_speed_{1.0f};
    static constexpr float camera_rotation_speed_{90.0f};
};

class Sandbox : public Hazel::Application {
public:
    Sandbox() { pushLayer(std::make_unique<ExampleLayer>()); }
    ~Sandbox() = default;
    Sandbox& operator=(Sandbox&&) noexcept = delete;
};

Hazel::Application* Hazel::CreateApplication() { return new Sandbox(); }
