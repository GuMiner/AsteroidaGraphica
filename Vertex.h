#pragma once

// List of vertex types that are sendable to OpenGL
#include <vector>
#include <glbinding/gl/gl.h>
#include "vmath.hpp"

using namespace gl;

struct universalVertices
{
private:
	// Sends floating-point vec2/3/4 data to OpenGL
    template<typename T>
    static void SendToOpenGl(GLuint buffer, GLuint shaderIdx, GLuint itemCount, const std::vector<T>& data);

	// Sends indexed unsigned integer index data to OpenGL
	static void SendUIntToOpenGl(GLuint buffer, GLuint shaderIdx, GLuint itemCount, const std::vector<unsigned int>& data);

	// Sends indicies for indexed drawing to OpenGL
	static void SendIndicesToOpenGl(GLuint buffer, const std::vector<unsigned int>& data);

public:
    std::vector<vmath::vec3> positions;
    std::vector<vmath::vec3> colors;
    std::vector<vmath::vec3> barycentrics;
    std::vector<vmath::vec2> uvs;
    std::vector<unsigned int> ids;

	std::vector<unsigned int> indices;

    // Adds a position, color, UV vertex.
    void AddColorTextureVertex(vmath::vec3 position, vmath::vec3 color, vmath::vec2 uv);

    // Transfers the vertex data for any non-zero filled array into OpenGL.
    // Note that buffer layout is 1 == position, 2 == color, and so on and so forth.
    static void TransferToOpenGl(const universalVertices& vertices, GLuint positionBuffer, GLuint colorBuffer,
		GLuint barycentricBuffer, GLuint uvBuffer, GLuint idBuffer, GLuint indiciesBuffer);
};

struct DrawArraysIndirectCommand
{
    GLuint vertexCount;
    GLuint instanceCount;
    GLuint firstVertexOffset;
    GLuint baseInstance;

    void Set(GLuint vertexCount, GLuint instanceCount, GLuint firstVertexOffset, GLuint baseInstance);
};