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
    // color = texture(u_textures[int(v_tex_index)], v_tex_coord * v_tiling_factor) * v_color;
    // apparently the above doesn't work on some AMD graphics cards - have to branch explicitly
    vec4 texColor = v_color;
    switch(int(v_tex_index))
    {
        case 0: texColor *= texture(u_textures[0], v_tex_coord * v_tiling_factor); break;
        case 1: texColor *= texture(u_textures[1], v_tex_coord * v_tiling_factor); break;
        case 2: texColor *= texture(u_textures[2], v_tex_coord * v_tiling_factor); break;
        case 3: texColor *= texture(u_textures[3], v_tex_coord * v_tiling_factor); break;
        case 5: texColor *= texture(u_textures[5], v_tex_coord * v_tiling_factor); break;
        case 6: texColor *= texture(u_textures[6], v_tex_coord * v_tiling_factor); break;
        case 7: texColor *= texture(u_textures[7], v_tex_coord * v_tiling_factor); break;
        case 8: texColor *= texture(u_textures[8], v_tex_coord * v_tiling_factor); break;
        case 9: texColor *= texture(u_textures[9], v_tex_coord * v_tiling_factor); break;
        case 10: texColor *= texture(u_textures[10], v_tex_coord * v_tiling_factor); break;
        case 11: texColor *= texture(u_textures[11], v_tex_coord * v_tiling_factor); break;
        case 12: texColor *= texture(u_textures[12], v_tex_coord * v_tiling_factor); break;
        case 13: texColor *= texture(u_textures[13], v_tex_coord * v_tiling_factor); break;
        case 14: texColor *= texture(u_textures[14], v_tex_coord * v_tiling_factor); break;
        case 15: texColor *= texture(u_textures[15], v_tex_coord * v_tiling_factor); break;
        case 16: texColor *= texture(u_textures[16], v_tex_coord * v_tiling_factor); break;
        case 17: texColor *= texture(u_textures[17], v_tex_coord * v_tiling_factor); break;
        case 18: texColor *= texture(u_textures[18], v_tex_coord * v_tiling_factor); break;
        case 19: texColor *= texture(u_textures[19], v_tex_coord * v_tiling_factor); break;
        case 20: texColor *= texture(u_textures[20], v_tex_coord * v_tiling_factor); break;
        case 21: texColor *= texture(u_textures[21], v_tex_coord * v_tiling_factor); break;
        case 22: texColor *= texture(u_textures[22], v_tex_coord * v_tiling_factor); break;
        case 23: texColor *= texture(u_textures[23], v_tex_coord * v_tiling_factor); break;
        case 24: texColor *= texture(u_textures[24], v_tex_coord * v_tiling_factor); break;
        case 25: texColor *= texture(u_textures[25], v_tex_coord * v_tiling_factor); break;
        case 26: texColor *= texture(u_textures[26], v_tex_coord * v_tiling_factor); break;
        case 27: texColor *= texture(u_textures[27], v_tex_coord * v_tiling_factor); break;
        case 28: texColor *= texture(u_textures[28], v_tex_coord * v_tiling_factor); break;
        case 29: texColor *= texture(u_textures[29], v_tex_coord * v_tiling_factor); break;
        case 30: texColor *= texture(u_textures[30], v_tex_coord * v_tiling_factor); break;
        case 31: texColor *= texture(u_textures[31], v_tex_coord * v_tiling_factor); break;
    }
    color = texColor;
}
