#include "Vertex.h"
#include <stddef.h>

void colorVertex::Set(float x, float y, float z, float r, float g, float b)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
}

// Transfers the specified amount of vertices, in the correct format, to the GL_ARRAY_BUFFER
void colorVertex::TransferToOpenGl(colorVertex* vertices, GLsizei vertexCount)
{
    // Setup of how the GPU will understand our data we send to it.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(colorVertex), (GLvoid*)offsetof(colorVertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(colorVertex), (GLvoid*)offsetof(colorVertex, r));
    glEnableVertexAttribArray(1);

    // Send the data to OpenGL
    glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(colorVertex), vertices, GL_DYNAMIC_DRAW);
}

void colorBarycentricVertex::Set(float x, float y, float z, float r, float g, float b, float xb, float yb, float zb)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->xb = xb;
    this->yb = yb;
    this->zb = zb;
}

void colorBarycentricVertex::TransferToOpenGl(colorBarycentricVertex* vertices, GLsizei vertexCount)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(colorBarycentricVertex), (GLvoid*)offsetof(colorBarycentricVertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(colorBarycentricVertex), (GLvoid*)offsetof(colorBarycentricVertex, r));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(colorBarycentricVertex), (GLvoid*)offsetof(colorBarycentricVertex, xb));
    glEnableVertexAttribArray(2);

    glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(colorBarycentricVertex), vertices, GL_DYNAMIC_DRAW);
}

void colorTextureVertex::Set(float x, float y, float z, float r, float g, float b, float u, float v)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->u = u;
    this->v = v;
}

// Transfers the specified amount of vertices, in the correct format, to the GL_ARRAY_BUFFER
void colorTextureVertex::TransferToOpenGl(colorTextureVertex* vertices, GLsizei vertexCount)
{
    // Setup of how the GPU will understand our data we send to it.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(colorTextureVertex), (GLvoid*)offsetof(colorTextureVertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(colorTextureVertex), (GLvoid*)offsetof(colorTextureVertex, r));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(colorTextureVertex), (GLvoid*)offsetof(colorTextureVertex, u));
    glEnableVertexAttribArray(2);

    // Send the data to OpenGL
    glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(colorTextureVertex), vertices, GL_DYNAMIC_DRAW);
}