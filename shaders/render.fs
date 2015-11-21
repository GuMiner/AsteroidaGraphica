#version 430 core 

out vec4 color;

in VS_OUT
{
    vec4 color;
    vec3 position;
    flat vec3 cornerPos;
} fs_in;

void main(void)
{
    vec4 inpColor = fs_in.color;

    float dist = distance(fs_in.position, fs_in.cornerPos);

    inpColor.r = cos(dist*12);
    inpColor.g = cos(dist*12);
    inpColor.b = cos(dist*12);
    color = inpColor;
}