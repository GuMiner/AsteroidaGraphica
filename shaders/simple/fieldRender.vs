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
	float speed = 2.0;
	float frequency = 60.0f;
	float freqMult = 20.0f;
	float factor = cos(frequency*length(position.xy) + phase*speed);
	vs_out.color = vec4(max(factor*0.7, 0.3), max(factor*0.7, 0.3), max(factor, 0.5), 1.0f);
    gl_Position = projMatrix * vec4(position, 1.0f);
}
