#version 400 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec4 barycentricPosition;

out VS_OUT
{
    vec4 barycentricPosition;
} vs_out;

uniform mat4 proj_matrix;

// Renders asteroids with the specified color and position, using the barycentric coordinates to determine the opacity level.
void main(void)
{
    vs_out.barycentricPosition = barycentricPosition;

    gl_Position = proj_matrix * vec4(position, 1);
}
