#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 barycentricPosition;

uniform sampler1D asteroidPositions;

out VS_OUT
{
    vec4 color;
    vec3 barycentricPosition;
} vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

// Renders asteroids with the specified color and position, using the barycentric coordinates to determine the opacity level.
void main(void)
{
    vs_out.color = vec4(1.0f, 1.0f, 0.0f, 1);
    vs_out.barycentricPosition = barycentricPosition;

    vec4 asteroidPosition = texture(asteroidPositions, gl_InstanceID);
    gl_Position = proj_matrix * mv_matrix * (vec4(position, 0) + asteroidPosition);
}
