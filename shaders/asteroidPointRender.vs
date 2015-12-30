#version 400 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;

out VS_OUT
{
    vec4 color;
} vs_out;

uniform mat4 proj_matrix;

// Renders asteroid points in as simple a matter as possible.
void main(void)
{
    vs_out.color = vec4(color, 1);
    gl_Position = proj_matrix * vec4(position.xyz, 1);
	gl_PointSize = 8.0f;
}
