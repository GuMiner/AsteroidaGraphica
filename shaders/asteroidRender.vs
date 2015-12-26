#version 400 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec4 barycentricPosition;
layout (location = 4) in uint drawId;

uniform sampler1D asteroidPositions;

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
    vs_out.color = vec4(1.0f, 1.0f, 0.0f, 1);
    vs_out.barycentricPosition = barycentricPosition;

    vec4 asteroidPosition = texelFetch(asteroidPositions, int(drawId), 0);
    gl_Position = proj_matrix * mv_matrix * (vec4(position, 1) + asteroidPosition);
}
