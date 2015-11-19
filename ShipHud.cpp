#include "ShipHud.h"

ShipHud::ShipHud()
{
    compassSize = 1.0f;
    float compassDepth = -8.2f;
    float compassHeight = -3.2f;

    xyCompassTranslation = vmath::translate(-6.0f, compassHeight, compassDepth);
    yzCompassTranslation = vmath::translate(-4.8f, compassHeight, compassDepth);
    zxCompassTranslation = vmath::translate(-3.6f, compassHeight, compassDepth);

    xyzCompassRotations = vmath::vec3(0, 0.0f, 0.0f);

    textTranslation = vmath::translate(-4.0f, -1.0f, -6.0f);
}

void ShipHud::Initialize(FontManager* fontManager, GLuint compassTexture, GLint projLocation, GLint mvLocation)
{
    this->fontManager = fontManager;

    this->compassTexture = compassTexture;
    this->projLocation = projLocation;
    this->mvLocation = mvLocation;

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
    LoadCompassIndicator(pVertices, xyCompassOffset, vmath::vec3(1.0f, 0, 0));

    // Add in the yz compass data
    yzCompassOffset = compassVertexCount;
    LoadCompassIndicator(pVertices, yzCompassOffset, vmath::vec3(0, 1.0f, 0));

    // Add in the zx compass data
    zxCompassOffset = compassVertexCount * 2;
    LoadCompassIndicator(pVertices, zxCompassOffset, vmath::vec3(0, 0, 1.0f));

    colorTextureVertex::TransferToOpenGl(pVertices, totalVertexCount);
    delete[] pVertices;


    // Now create our text information.
    glGenVertexArrays(1, &textVao);
    glBindVertexArray(textVao);

    glGenBuffers(1, &textVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textVertexBuffer);

    std::string sentence = std::string("ABCDEFGHIJKmnopq7%");
    textVertexCount = fontManager->GetSentenceVertexCount(sentence);
    colorTextureVertex *textVertices = fontManager->AllocateSentenceVertices(sentence, 80, vmath::vec3(1.0f, 1.0f, 1.0f));

    colorTextureVertex::TransferToOpenGl(textVertices, textVertexCount);
    delete[] textVertices;
}

// Loads in a compass indicator into the currently-active vertex buffer.
void ShipHud::LoadCompassIndicator(colorTextureVertex *pVertices, GLsizei offset, vmath::vec3 colorMax)
{
    pVertices[0 + offset].Set(-compassSize / 2, -compassSize / 2, 0, 0, 0, 0, 0, 1);
    pVertices[1 + offset].Set(-compassSize / 2, compassSize / 2, 0, colorMax[0], colorMax[1], colorMax[2], 0, 0);
    pVertices[2 + offset].Set(-compassSize / 2 + compassSize, -compassSize / 2, 0, 0, 0, 0, 1, 1);
                                                                                   
    pVertices[3 + offset].Set(compassSize / 2, -compassSize / 2, 0, 0, 0, 0, 1, 1);
    pVertices[4 + offset].Set(-compassSize / 2, compassSize / 2, 0, colorMax[0], colorMax[1], colorMax[2], 0, 0);
    pVertices[5 + offset].Set(compassSize / 2, compassSize / 2, 0, colorMax[0], colorMax[1], colorMax[2], 1, 0);
}

// Updates the compass rotations; expects rotations in radians.
void ShipHud::UpdateCompassRotations(vmath::vec3& compassRotations)
{
    xyzCompassRotations = vmath::vec3(vmath::degrees(compassRotations[0]), vmath::degrees(compassRotations[1]), vmath::degrees(compassRotations[2]));
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
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, xyCompassTranslation * vmath::rotate(xyzCompassRotations[0], vmath::vec3(0, 0, -1)));

    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, xyCompassOffset, compassVertexCount);

    // YZ compass
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, yzCompassTranslation * vmath::rotate(xyzCompassRotations[1], vmath::vec3(0, 0, -1)));

    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, yzCompassOffset, compassVertexCount);

    // ZX compass
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, zxCompassTranslation * vmath::rotate(xyzCompassRotations[2], vmath::vec3(0, 0, -1)));

    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, zxCompassOffset, compassVertexCount);

    // TODO testing text
    fontManager->RenderSentenceVertices(textVao, textVertexBuffer, projLocation, mvLocation, perspectiveMatrix, textTranslation, textVertexCount);
}


ShipHud::~ShipHud()
{
    // Free up the ship HUD VAO and VBO
    glDeleteVertexArrays(1, &compassVao);
    glDeleteBuffers(1, &compassVertexBuffer);

    glDeleteVertexArrays(1, &textVao);
    glDeleteBuffers(1, &textVertexBuffer);
}
