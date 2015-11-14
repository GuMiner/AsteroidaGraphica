#include "vmath.hpp"
#include "ShipHud.h"

#include <SFML\Window.hpp>

ShipHud::ShipHud()
{
    compassSize = 1.0f;
    compassDepth = -8.2f;
    
    xyCompassX = -6.6f;
    xyCompassY = -3.2f;

    yzCompassX = -5.4f;
    yzCompassY = -3.2f;

    zxCompassX = -4.2f;
    zxCompassY = -3.2f;
}

void ShipHud::Initialize(GLuint compassTexture, GLint projLocation)
{
    this->compassTexture = compassTexture;
    this->projLocation = projLocation;

    // Create our general compass information
    compassVertexCount = 6;

    glGenVertexArrays(1, &compassVao);
    glBindVertexArray(compassVao);
    
    glGenBuffers(1, &compassVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);

    GLsizei totalVertexCount = 18;
    colorTextureVertex *pVertices = new colorTextureVertex[totalVertexCount];

    // Add in the xy compass data
    xyCompassOffset = 0;
    LoadCompassIndicator(pVertices, xyCompassOffset, xyCompassX, xyCompassY, vmath::vec3(1.0f, 0, 0));

    // Add in the yz compass data
    yzCompassOffset = compassVertexCount;
    LoadCompassIndicator(pVertices, yzCompassOffset, yzCompassX, yzCompassY, vmath::vec3(0, 1.0f, 0));

    // Add in the zx compass data
    zxCompassOffset = compassVertexCount * 2;
    LoadCompassIndicator(pVertices, zxCompassOffset, zxCompassX, zxCompassY, vmath::vec3(0, 0, 1.0f));

    colorTextureVertex::TransferToOpenGl(pVertices, totalVertexCount);
    delete[] pVertices;
}

// Loads in a compass indicator into the currently-active vertex buffer.
void ShipHud::LoadCompassIndicator(colorTextureVertex *pVertices, GLsizei offset, float compassX, float compassY, vmath::vec3 colorMax)
{
    pVertices[0 + offset].Set(compassX, compassY, compassDepth,                             0, 0, 0, 0, 1);
    pVertices[1 + offset].Set(compassX, compassY + compassSize, compassDepth,               colorMax[0], colorMax[1], colorMax[2], 0, 0);
    pVertices[2 + offset].Set(compassX + compassSize, compassY, compassDepth,               0, 0, 0, 1, 1);
                                                                                   
    pVertices[3 + offset].Set(compassX + compassSize, compassY, compassDepth,               0, 0, 0, 1, 1);
    pVertices[4 + offset].Set(compassX, compassY + compassSize, compassDepth,               colorMax[0], colorMax[1], colorMax[2], 0, 0);
    pVertices[5 + offset].Set(compassX + compassSize, compassY + compassSize, compassDepth, colorMax[0], colorMax[1], colorMax[2], 1, 0);
}

// Renders the HUD of the ship.
void ShipHud::RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock)
{
    // Bind in the texture and vertices we're using.
    glBindVertexArray(compassVao);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, compassTexture);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, perspectiveMatrix);

    // Draw the XY compass.
    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, xyCompassOffset, compassVertexCount);

    // YZ compass
    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, yzCompassOffset, compassVertexCount);

    // ZX compass
    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, zxCompassOffset, compassVertexCount);
}


ShipHud::~ShipHud()
{
    // Free up the ship HUD vertex buffer
    glDeleteVertexArrays(1, &compassVao);

    glDeleteBuffers(1, &compassVertexBuffer);
}
