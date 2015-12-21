#pragma once

// List of vertex types that are sendable to OpenGL
#include <GL\glew.h>
#include <vector>
#include "vmath.hpp"

struct universalVertices
{
private:
    template<typename T>
    static void SendToOpenGl(GLuint buffer, GLuint shaderIdx, GLuint itemCount, const std::vector<T>& data);

public:
    std::vector<vmath::vec3> positions;
    std::vector<vmath::vec3> colors;
    std::vector<vmath::vec3> barycentrics;
    std::vector<vmath::vec2> uvs;
    std::vector<float> ids;

    // Adds a position, color, UV vertex.
    void AddColorTextureVertex(vmath::vec3 position, vmath::vec3 color, vmath::vec2 uv);

    // Transfers the vertex data for any non-zero filled array into OpenGL.
    // Note that buffer layout is 1 == position, 2 == color, and so on and so forth.
    static void TransferToOpenGl(const universalVertices& vertices, GLuint positionBuffer, GLuint colorBuffer, GLuint barycentricBuffer, GLuint uvBuffer, GLuint idBuffer);
};

struct barycentricVertex
{
    float x;
    float y;
    float z;
    float xb;
    float yb;
    float zb;

    void Set(float x, float y, float z, float xb, float yb, float zb);

    // Transfers the specified amount of vertices, in the correct format, to the GL_ARRAY_BUFFER
    static void TransferToOpenGl(barycentricVertex* vertices, GLsizei vertexCount);
};

struct DrawArraysIndirectCommand
{
    GLuint vertexCount;
    GLuint instanceCount;
    GLuint firstVertexOffset;
    GLuint baseInstance;

    void Set(GLuint vertexCount, GLuint instanceCount, GLuint firstVertexOffset, GLuint baseInstance);
};