#version 400 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec4 barycentricPosition;
layout (location = 4) in uint drawId;

uniform sampler1D asteroidPositions;
uniform sampler1D asteroidColors;
uniform sampler1D asteroidRotations;

out VS_OUT
{
    vec4 color;
    vec4 barycentricPosition;
} vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

// Renders asteroids with the specified color and position, using the barycentric coordinates to determine the opacity level.
void main(void)
{
	// Pass-through colors and barycentrics to our fragment shader.
    vs_out.color = vec4(texelFetch(asteroidColors, int(drawId), 0).xyz, 1);
    vs_out.barycentricPosition = barycentricPosition;

	// Form a matrix out of our asteroid position because we perform rotation beforehand, so we can't add it to the existing vertex position.
    vec4 asteroidPosition = texelFetch(asteroidPositions, int(drawId), 0);
	mat4 asteroidPositionMatrix = mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		asteroidPosition.x, asteroidPosition.y, asteroidPosition.z, 1.0f);

	// Deconstruct the asteroid rotation quaternion into a matrix.
	// This is computationally expensive, but because we're limited to 8 texture units on low-spec hardware, we don't have much choice.
	vec4 asteroidRotation = texelFetch(asteroidRotations, int(drawId), 0);
	float xx = asteroidRotation.x * asteroidRotation.x;
	float yy = asteroidRotation.y * asteroidRotation.y;
	float zz = asteroidRotation.z * asteroidRotation.z;
	
	float xy = asteroidRotation.x * asteroidRotation.y;
	float xz = asteroidRotation.x * asteroidRotation.z;
	float xw = asteroidRotation.x * asteroidRotation.a;
	
	float yz = asteroidRotation.y * asteroidRotation.z;
	float yw = asteroidRotation.y * asteroidRotation.w;
	
	float zw = asteroidRotation.z * asteroidRotation.w;

	mat4 asteroidRotationMatrix = mat4(
		1.0f - 2.0f * (yy + zz), 2.0f * (xy - zw), 2.0f * (xz + yw), 0.0f,
		2.0f * (xy + zw), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - xw), 0.0f,
		2.0f * (xz - yw), 2.0f * (yz + xw), 1.0f - 2.0f * (xx + yy), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//asteroidRotationMatrix = transpose(asteroidRotationMatrix);
    gl_Position = proj_matrix * mv_matrix * asteroidPositionMatrix * asteroidRotationMatrix * vec4(position, 1);
}
