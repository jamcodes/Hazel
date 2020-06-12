#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"

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

}  // namespace

namespace Hazel
{

class HazelEditor : public Application {
public:
    HazelEditor() : Application{"HazelEditor"} {
        pushLayer(std::make_unique<EditorLayer>());
    }
    ~HazelEditor() = default;
    HazelEditor& operator=(HazelEditor&&) noexcept = delete;
};

} // namespace Hazel

Hazel::Application* Hazel::CreateApplication() { return new Hazel::HazelEditor{}; }
