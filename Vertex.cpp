#include "Vertex.h"
#include <stddef.h>

template<typename T>
void universalVertices::SendToOpenGl(GLuint buffer, GLuint shaderIdx, GLuint itemCount, const std::vector<T>& data)
{
    glEnableVertexAttribArray(shaderIdx);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(shaderIdx, itemCount, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), &data[0], GL_STATIC_DRAW);
}

void universalVertices::SendUIntToOpenGl(GLuint buffer, GLuint shaderIdx, GLuint itemCount, const std::vector<unsigned int>& data)
{
	glEnableVertexAttribArray(shaderIdx);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Note this is glVertexAttrib*I*Pointer in comparison to the other call above.
	glVertexAttribIPointer(shaderIdx, itemCount, GL_UNSIGNED_INT, 0, nullptr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
}

void universalVertices::SendIndicesToOpenGl(GLuint buffer, const std::vector<unsigned int>& data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
}

void universalVertices::AddColorTextureVertex(vmath::vec3 position, vmath::vec3 color, vmath::vec2 uv)
{
    positions.push_back(position);
    colors.push_back(color);
    uvs.push_back(uv);
}

void universalVertices::TransferToOpenGl(const universalVertices& vertices, GLuint positionBuffer, GLuint colorBuffer,
	GLuint barycentricBuffer, GLuint uvBuffer, GLuint idBuffer, GLuint indiciesBuffer)
{
    if (vertices.positions.size() != 0)
    {
        universalVertices::SendToOpenGl(positionBuffer, 0, 3, vertices.positions);
    }

    if (vertices.colors.size() != 0)
    {
        universalVertices::SendToOpenGl(colorBuffer, 1, 3, vertices.colors);
    }

    if (vertices.barycentrics.size() != 0)
    {
        universalVertices::SendToOpenGl(barycentricBuffer, 2, 3, vertices.barycentrics);
    }

    if (vertices.uvs.size() != 0)
    {
        universalVertices::SendToOpenGl(uvBuffer, 3, 2, vertices.uvs);
    }

    if (vertices.ids.size() != 0)
    {
        universalVertices::SendUIntToOpenGl(idBuffer, 4, 1, vertices.ids);
    }

	if (vertices.indices.size() != 0)
	{
		universalVertices::SendIndicesToOpenGl(indiciesBuffer, vertices.indices);
	}
}

void DrawArraysIndirectCommand::Set(GLuint vertexCount, GLuint instanceCount, GLuint firstVertexOffset, GLuint baseInstance)
{
    this->vertexCount = vertexCount;
    this->instanceCount = instanceCount;
    this->firstVertexOffset = firstVertexOffset;
    this->baseInstance = baseInstance;
}