#include "vmath.hpp"
#include "ShipHud.h"
#include "Vertex.h"

ShipHud::ShipHud()
{
}

void ShipHud::Initialize(GLuint compassTexture, GLint projLocation)
{
    this->compassTexture = compassTexture;
    this->projLocation = projLocation;

    // Setup the transfer chains.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // TEST CODE -- this should be updated probably every turn based on the ships position and orientatino.
    colorTextureVertex *pVertices = new colorTextureVertex[6];
    pVertices[0].Set(0, 0, -4, 1, 1, 1, 0, 1);
    pVertices[1].Set(0, 1, -4, 0, 1, 0, 0, 0);
    pVertices[2].Set(1, 0, -4, 1, 0, 0, 1, 1);

    pVertices[3].Set(1, 0, -4, 1, 1, 1, 1, 1);
    pVertices[4].Set(0, 1, -4, 0, 1, 0, 0, 0);
    pVertices[5].Set(1, 1, -4, 1, 0, 0, 1, 0);

    vertexCount = 6;
    colorTextureVertex::TransferToOpenGl(pVertices, vertexCount);
    delete[] pVertices;
}

// Renders the HUD of the ship.
void ShipHud::RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock)
{
    // Bind in the texture and vertices we're using.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, compassTexture);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, perspectiveMatrix);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


ShipHud::~ShipHud()
{
    // Free up the ship HUD vertex buffer
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexBuffer);
}
