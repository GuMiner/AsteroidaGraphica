#include <string>
#include <sstream>
#include "Logger.h"
#include "ShipHud.h"
#include <SFML\Window.hpp>

ShipHud::ShipHud()
{
    hudDepth = -1.0f;
    compassSize = 1.0f;
    float compassDepth = -8.2f;
    float compassHeight = -3.2f;

    xyCompassMatrix = vmath::translate(-6.0f, compassHeight, compassDepth);
    yzCompassMatrix = vmath::translate(-4.8f, compassHeight, compassDepth);
    zxCompassMatrix = vmath::translate(-3.6f, compassHeight, compassDepth);

    xyzCompassRotations = vmath::vec3(0.0f, 0.0f, 0.0f);

    textScale = vmath::scale(0.02f, 0.02f, 0.02f);
    
    xTextMatrix = vmath::translate(-0.821f, -0.321f, hudDepth) * textScale;
    yTextMatrix = vmath::translate(-0.659f, -0.321f, hudDepth) * textScale;
    zTextMatrix = vmath::translate(-0.508f, -0.321f, hudDepth) * textScale;

    xPosTextMatrix = vmath::translate(-0.821f, -0.296f, hudDepth) * textScale;
    yPosTextMatrix = vmath::translate(-0.659f, -0.296f, hudDepth) * textScale;
    zPosTextMatrix = vmath::translate(-0.508f, -0.296f, hudDepth) * textScale;
}

bool ShipHud::Initialize(ShaderManager* shaderManager, FontManager* fontManager, ImageManager* imageManager)
{
    this->fontManager = fontManager;

    // Load the image texture and image shader
    
    Logger::Log("Loading compass texture shader program...");
    if (!shaderManager->CreateShaderProgram("texRender", &compassTextureProgram))
    {
        return false;
    }
    mvLocation = glGetUniformLocation(compassTextureProgram, "mv_matrix");
    projLocation = glGetUniformLocation(compassTextureProgram, "proj_matrix");
    Logger::Log("Compass shader loading complete!");

    Logger::Log("Loading compass texture...");
    compassTexture = imageManager->AddImage("images/DirectionDial.png");
    if (compassTexture == 0)
    {
        return false;
    }

    Logger::Log("Compass texture image loading done!");
    
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

    xSentence = fontManager->CreateNewSentence();
    ySentence = fontManager->CreateNewSentence();
    zSentence = fontManager->CreateNewSentence();

    xPosSentence = fontManager->CreateNewSentence();
    yPosSentence = fontManager->CreateNewSentence();
    zPosSentence = fontManager->CreateNewSentence();

    return true;
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
    
    std::stringstream textOutputStream;
    textOutputStream.precision(3);
    textOutputStream << std::fixed;

    textOutputStream << "XY: " << xyzCompassRotations[0];
    fontManager->UpdateSentence(xSentence, textOutputStream.str(), 20, vmath::vec3(1.0f, 1.0f, 0.0f));

    textOutputStream.str("");
    textOutputStream << "YZ: " << xyzCompassRotations[1];
    fontManager->UpdateSentence(ySentence, textOutputStream.str(), 20, vmath::vec3(0.0f, 1.0f, 1.0f));

    textOutputStream.str("");
    textOutputStream << "ZX: " << xyzCompassRotations[2];
    fontManager->UpdateSentence(zSentence, textOutputStream.str(), 20, vmath::vec3(1.0f, 0.0f, 1.0f));
}

// Updates the ship position text.
void ShipHud::UpdateShipPositition(vmath::vec3& shipPosition)
{
    std::stringstream textOutputStream;
    textOutputStream.precision(2);
    textOutputStream << std::fixed;

    textOutputStream << "X: " << shipPosition[0];
    fontManager->UpdateSentence(xPosSentence, textOutputStream.str(), 20, vmath::vec3(1.0f, 0.0f, 0.0f));

    textOutputStream.str("");
    textOutputStream << "Y: " << shipPosition[1];
    fontManager->UpdateSentence(yPosSentence, textOutputStream.str(), 20, vmath::vec3(0.0f, 1.0f, 0.0f));

    textOutputStream.str("");
    textOutputStream << "Z: " << shipPosition[1];
    fontManager->UpdateSentence(zPosSentence, textOutputStream.str(), 20, vmath::vec3(0.0f, 0.0f, 1.0f));
}

// Renders the HUD of the ship.
void ShipHud::RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock)
{
    // Bind in the texture and vertices we're using.
    glUseProgram(compassTextureProgram);
    glBindVertexArray(compassVao);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, compassTexture);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, perspectiveMatrix);

    // Draw the XY compass.
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, xyCompassMatrix * vmath::rotate(xyzCompassRotations[0], vmath::vec3(0, 0, -1)));

    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, xyCompassOffset, compassVertexCount);

    // YZ compass
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, yzCompassMatrix * vmath::rotate(xyzCompassRotations[1], vmath::vec3(0, 0, -1)));

    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, yzCompassOffset, compassVertexCount);

    // ZX compass
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, zxCompassMatrix * vmath::rotate(xyzCompassRotations[2], vmath::vec3(0, 0, -1)));

    glBindBuffer(GL_ARRAY_BUFFER, compassVertexBuffer);
    glDrawArrays(GL_TRIANGLES, zxCompassOffset, compassVertexCount);

    // Draw our sentences
    fontManager->RenderSentence(xSentence, perspectiveMatrix, xTextMatrix);
    fontManager->RenderSentence(ySentence, perspectiveMatrix, yTextMatrix);
    fontManager->RenderSentence(zSentence, perspectiveMatrix, zTextMatrix);
    
    fontManager->RenderSentence(xPosSentence, perspectiveMatrix, xPosTextMatrix);
    fontManager->RenderSentence(yPosSentence, perspectiveMatrix, yPosTextMatrix);
    fontManager->RenderSentence(zPosSentence, perspectiveMatrix, zPosTextMatrix);
}

ShipHud::~ShipHud()
{
    // Free up the ship HUD VAO and VBO
    glDeleteVertexArrays(1, &compassVao);
    glDeleteBuffers(1, &compassVertexBuffer);
}
