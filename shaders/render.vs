#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out VS_OUT
{
    vec4 color;
} vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
    vec4 pos = vec4(position, 1);

    gl_Position = proj_matrix * mv_matrix * pos;
    
    // Output stuff to the fragment shader
    vs_out.color = vec4(color.x*sign(color.x), color.y*sign(color.y), color.z*sign(color.z), 1);
}