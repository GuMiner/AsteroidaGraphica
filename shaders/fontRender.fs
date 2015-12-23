#version 400 core 

uniform sampler2D imageTex;

out vec4 color;

in VS_OUT
{
    vec4 color;
    vec2 texPos;
} fs_in;

void main(void)
{
    // Scale each color of the provided object by the given color.
    color = fs_in.color * texture(imageTex, fs_in.texPos);
}