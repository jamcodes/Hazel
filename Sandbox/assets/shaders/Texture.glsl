#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex_coord;
layout(location = 3) in float a_tex_index;
layout(location = 4) in float a_tiling_factor;

uniform mat4 u_view_projection;
// uniform mat4 u_transform;

out vec4 v_color;
out vec2 v_tex_coord;
out float v_tex_index;
out float v_tiling_factor;

void main()
{
    v_color = a_color;
    v_tex_coord = a_tex_coord;
    v_tex_index = a_tex_index;
    v_tiling_factor = a_tiling_factor;
    // gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
    gl_Position = u_view_projection * vec4(a_position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_tex_coord;
in float v_tex_index;
in float v_tiling_factor;

// uniform vec4 u_color;
// uniform float u_tiling_factor;
uniform sampler2D u_textures[32];

void main()
{
    // TODO: u_tiling_factor - needs to be handled in the vertex
    color = texture(u_textures[int(v_tex_index)], v_tex_coord * v_tiling_factor) * v_color;
}
