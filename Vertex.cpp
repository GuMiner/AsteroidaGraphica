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

void universalVertices::AddColorTextureVertex(vmath::vec3 position, vmath::vec3 color, vmath::vec2 uv)
{
    positions.push_back(position);
    colors.push_back(color);
    uvs.push_back(uv);
}

void universalVertices::TransferToOpenGl(const universalVertices& vertices, GLuint positionBuffer, GLuint colorBuffer, GLuint barycentricBuffer, GLuint uvBuffer, GLuint idBuffer)
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
        universalVertices::SendToOpenGl(idBuffer, 4, 1, vertices.ids);
    }
}

void barycentricVertex::Set(float x, float y, float z, float xb, float yb, float zb)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->xb = xb;
    this->yb = yb;
    this->zb = zb;
}

void barycentricVertex::TransferToOpenGl(barycentricVertex* vertices, GLsizei vertexCount)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(barycentricVertex), (GLvoid*)offsetof(barycentricVertex, x));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(barycentricVertex), (GLvoid*)offsetof(barycentricVertex, xb));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(barycentricVertex), vertices, GL_DYNAMIC_DRAW);
}

void DrawArraysIndirectCommand::Set(GLuint vertexCount, GLuint instanceCount, GLuint firstVertexOffset, GLuint baseInstance)
{
    this->vertexCount = vertexCount;
    this->instanceCount = instanceCount;
    this->firstVertexOffset = firstVertexOffset;
    this->baseInstance = baseInstance;
}