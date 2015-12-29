#version 400 core

out vec4 color;

in VS_OUT
{
    vec4 color;
} fs_in;

void main(void)
{
	// Pass-through color fragment shader.
    color = fs_in.color;
}