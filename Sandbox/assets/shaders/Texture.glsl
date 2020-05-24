#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex_coord;

uniform mat4 u_view_projection;
// uniform mat4 u_transform;

out vec4 v_color;
out vec2 v_tex_coord;

void main()
{
    v_color = a_color;
    v_tex_coord = a_tex_coord;
    // gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
    gl_Position = u_view_projection * vec4(a_position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_tex_coord;

uniform vec4 u_color;
uniform float u_tiling_factor;
uniform sampler2D u_texture;

void main()
{
    // TODO: remove the *10 scale, or replace with a uniform if decide on keeping it
    // color = texture(u_texture, v_tex_coord * u_tiling_factor) * u_color;
    color = v_color;
}
