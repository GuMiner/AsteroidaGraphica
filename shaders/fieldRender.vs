#version 400 core

layout (location = 0) in vec3 position;

out VS_OUT
{
    vec4 color;
} vs_out;

uniform mat4 projMatrix;
uniform float phase;

void main(void)
{
    vs_out.color = vec4(phase, phase, phase, 1.0f);

    gl_Position = projMatrix * vec4(position, 1.0f);
}
