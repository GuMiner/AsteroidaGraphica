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
    vec4 texColor = texture(imageTex, fs_in.texPos);
    color = vec4(fs_in.color.r, fs_in.color.g, fs_in.color.b * texColor.b, fs_in.color.a);// * 
}