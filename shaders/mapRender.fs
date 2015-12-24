#version 400 core

uniform sampler2D imageTex;

out vec4 color;

in VS_OUT
{
    vec4 color;
    vec2 texPos;
} fs_in;

// Area that is covered by the indicator color.
uniform vec2 indicator_pos;
uniform vec2 indicator_size;
uniform vec3 indicator_color;

// Sets the color to be the given color multiplied by the texture image color, or 
//  the indicator color if we're in the indicator zone (80% transparency)
void main(void)
{
    if ((fs_in.texPos.x > indicator_pos.x - indicator_size.x && fs_in.texPos.x < indicator_pos.x + indicator_size.x)
        && (fs_in.texPos.y > indicator_pos.y - indicator_size.y && fs_in.texPos.y < indicator_pos.y + indicator_size.y))
    {
        color = vec4(indicator_color, 0.8f);
    }
    else
    {
        color = fs_in.color * texture(imageTex, fs_in.texPos);
    }
}