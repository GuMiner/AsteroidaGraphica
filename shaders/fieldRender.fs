#version 400 core

out vec4 color;

in VS_OUT
{
	vec3 position;
	flat vec3 cornerPos;
} fs_in;

uniform float phase;

void main(void)
{
	vec4 inpColor = vec4(1.0f);
	float dist = distance(fs_in.position, fs_in.cornerPos);

	inpColor.r = cos(dist*12 + phase*10);
	inpColor.g = cos(dist*12 + phase*10);
	inpColor.b = cos(dist*12 + phase*10);
    color = inpColor;
}