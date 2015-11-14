#pragma once

// List of vertex types that are sendable to OpenGL
#include <GL\glew.h>

struct colorVertex
{
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;

    void Set(float x, float y, float z, float r, float g, float b);

    // Transfers the specified amount of vertices, in the correct format, to the GL_ARRAY_BUFFER
    static void TransferToOpenGl(colorVertex* vertices, GLsizei vertexCount);
};

struct colorTextureVertex
{
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float u;
    float v;

    void Set(float x, float y, float z, float r, float g, float b, float u, float v);

    // Transfers the specified amount of vertices, in the correct format, to the GL_ARRAY_BUFFER
    static void TransferToOpenGl(colorTextureVertex* vertices, GLsizei vertexCount);
};