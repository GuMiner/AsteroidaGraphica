#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out VS_OUT
{
    vec4 color;
	vec3 position;
	flat vec3 cornerPos;
} vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
    // Perform our position and projection transformations, and pass-through the color.
	vs_out.color = vec4(color.x, color.y, color.z, 1);
	vs_out.position = position;
	vs_out.cornerPos = position;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1);
}