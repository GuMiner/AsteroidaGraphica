#version 400 core

layout (location = 0) in vec3 position;

out VS_OUT
{
	vec3 position;
	flat vec3 cornerPos;
} vs_out;

uniform mat4 projMatrix;

void main(void)
{
	vs_out.position = position;
	vs_out.cornerPos = position;

    gl_Position = projMatrix * vec4(position, 1.0f);
}
