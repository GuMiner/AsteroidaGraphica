#include "vmath.hpp"
#include "ShipHud.h"
#include "Vertex.h"

ShipHud::ShipHud()
{
}

void ShipHud::Initialize(GLuint compassTexture, GLint mvLocation, GLint projLocation)
{
    this->compassTexture = compassTexture;
    this->mvLocation = mvLocation;
    this->projLocation = projLocation;

    // Setup the transfer chains.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // TEST CODE -- this should be updated probably every turn based on the ships position and orientatino.
    colorTextureVertex *pVertices = new colorTextureVertex[6];
    pVertices[0].Set(0, 0, 0, 1, 1, 1, 0, 0);
    pVertices[1].Set(0, 7, 0, 0, 1, 0, 0, 1);
    pVertices[2].Set(7, 0, 0, 1, 0, 0, 1, 1);

    pVertices[3].Set(0, 0, 0, 1, 1, 1, 0, 0);
    pVertices[4].Set(0, 0, 7, 0, 1, 0, 1, 0);
    pVertices[5].Set(0, 7, 0, 1, 0, 0, 1, 1);

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

    vmath::mat4 lookAtMatrix = vmath::lookat(vmath::vec3(8, 0, 8), vmath::vec3(0, 0, 0), vmath::vec3(0, 0, 1));
    vmath::mat4 result = perspectiveMatrix * lookAtMatrix;
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, result);

    // No translation
    vmath::mat4 mv_matrix = vmath::rotate(0.0f, 0.0f, ((float)clock.getElapsedTime().asMilliseconds() / 1000.0f) * 90.0f);
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


ShipHud::~ShipHud()
{
    // Free up the ship HUD vertex buffer
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexBuffer);
}
