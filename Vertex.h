#pragma once
// List of vertex types that are sendable to OpenGL

struct colorVertex
{
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;

    void Set(float x, float y, float z, float r, float g, float b)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->r = r;
        this->g = g;
        this->b = b;
    }
};