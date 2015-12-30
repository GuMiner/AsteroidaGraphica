#version 400 core

out vec4 color;

in VS_OUT
{
	vec3 position;
} fs_in;

uniform float phase;

void main(void)
{
	vec4 inpColor = vec4(1.0f);
	vec3 normalized = normalize(fs_in.position);
	float distxy = normalized.x*normalized.x + normalized.y*normalized.y;
	float distyz = normalized.y*normalized.y + normalized.z*normalized.z;

	float speed = 2.0;
	float frequency = 40.0f;
	float freqMult = 20.0f;
	float factor = cos(distxy*frequency + phase*speed)*sin(distyz*frequency*freqMult + phase*speed);
	inpColor.r = max(factor*0.7, 0.3);
	inpColor.g = inpColor.r;
	inpColor.b = max(factor, 0.3);
    color = inpColor;
}