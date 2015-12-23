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
    
    mapBorderWidth = 0.05f; // Texture coordinates
    mapSize = 0.15f; // Physical coordinates
    positionIndicatorSize = (1.0f - (2.0f*mapBorderWidth)) / (30.0f * 2.0f); // Resolution of 30 map units.
    shipMapMatrix = vmath::translate(-0.36f, -0.45f, hudDepth);

    indicatorPosSize = vmath::vec3(0.5f, 0.4f, positionIndicatorSize);
    indicatorColor = vmath::vec3(0.0f, 1.0f, 1.0f);

    verticalMapMatrix = vmath::translate(-0.195f, -0.450f, hudDepth) * vmath::scale(0.20f, 1.0f, 1.0f);

    verticalIndicatorPosSize = vmath::vec3(0.5f, 0.4f, positionIndicatorSize);
    verticalIndicatorColor = vmath::vec3(0.0f, 1.0f, 1.0f);
}

void ShipHud::InitializeCompasses()
{
    glGenVertexArrays(1, &compassVao);
    glBindVertexArray(compassVao);

    glGenBuffers(1, &compassPositionBuffer);
    glGenBuffers(1, &compassColorBuffer);
    glGenBuffers(1, &compassUvBuffer);

    // Add in the xy, yz, and zx compass data
    universalVertices vertices;
    xyCompassOffset = vertices.positions.size();
    LoadCompassIndicator(vertices, vmath::vec3(1.0f, 0, 0));
    compassVertexCount = vertices.positions.size();

    zxCompassOffset = vertices.positions.size();
    LoadCompassIndicator(vertices, vmath::vec3(0, 1.0f, 0));

    yzCompassOffset = vertices.positions.size();
    LoadCompassIndicator(vertices, vmath::vec3(0, 0, 1.0f));
    universalVertices::TransferToOpenGl(vertices, compassPositionBuffer, compassColorBuffer, 0, compassUvBuffer, 0);
}

void ShipHud::InitializeShipMaps()
{
    // Load in the ship map vertex data.
    universalVertices shipMapVertices;

    shipMapVertices.AddColorTextureVertex(vmath::vec3(0, 0, 0), vmath::vec3(0, 1.0f, 0), vmath::vec2(0, 1));
    shipMapVertices.AddColorTextureVertex(vmath::vec3(mapSize, 0, 0), vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec2(1, 1));
    shipMapVertices.AddColorTextureVertex(vmath::vec3(mapSize, mapSize, 0), vmath::vec3(0, 0, 1.0f), vmath::vec2(1, 0));
    shipMapVertices.AddColorTextureVertex(vmath::vec3(0, mapSize, 0), vmath::vec3(1.0f, 1.0f, 1.0f), vmath::vec2(0, 0));
    shipMapVertexCount = shipMapVertices.positions.size();

    glGenVertexArrays(1, &shipMapVao);
    glBindVertexArray(shipMapVao);

    glGenBuffers(1, &shipMapPositionBuffer);
    glGenBuffers(1, &shipMapColorBuffer);
    glGenBuffers(1, &shipMapUvBuffer);
    
    universalVertices::TransferToOpenGl(shipMapVertices, shipMapPositionBuffer, shipMapColorBuffer, 0, shipMapUvBuffer, 0);

    // Load in the ship map vertex data for the vertical map.
    glGenVertexArrays(1, &verticalMapVao);
    glBindVertexArray(verticalMapVao);

    glGenBuffers(1, &verticalMapPositionBuffer);
    glGenBuffers(1, &verticalMapColorBuffer);
    glGenBuffers(1, &verticalMapUvBuffer);

    universalVertices::TransferToOpenGl(shipMapVertices, verticalMapPositionBuffer, verticalMapColorBuffer, 0, verticalMapUvBuffer, 0);
}

bool ShipHud::Initialize(ShaderManager* shaderManager, FontManager* fontManager, ImageManager* imageManager)
{
    this->fontManager = fontManager;

    // Load the shaders and textures.
    Logger::Log("Loading in the ship map shader program...");
    if (!shaderManager->CreateShaderProgram("mapRender", &shipMapShaderProgram))
    {
        return false;
    }

    shipMapMvLocation = glGetUniformLocation(shipMapShaderProgram, "mv_matrix");
    shipMapProjLocation = glGetUniformLocation(shipMapShaderProgram, "proj_matrix");
    shipMapTexLocation = glGetUniformLocation(shipMapShaderProgram, "imageTex");
    indicatorPosLocation = glGetUniformLocation(shipMapShaderProgram, "indicator_pos");
    indicatorSizeLocation = glGetUniformLocation(shipMapShaderProgram, "indicator_size");
    indicatorColorLocation = glGetUniformLocation(shipMapShaderProgram, "indicator_color");
    Logger::Log("Ship map loading complete!");

    Logger::Log("Loading ship map texture...");
    shipMapTexture = imageManager->AddImage("images/ShipMap.png");
    if (shipMapTexture == 0)
    {
        return false;
    }

    Logger::Log("Ship map texture loading done!");

    Logger::Log("Loading compass texture shader program...");
    if (!shaderManager->CreateShaderProgram("textureRender", &compassTextureProgram))
    {
        return false;
    }

    mvLocation = glGetUniformLocation(compassTextureProgram, "mv_matrix");
    projLocation = glGetUniformLocation(compassTextureProgram, "proj_matrix");
    compassTexLocation = glGetUniformLocation(compassTextureProgram, "imageTex");
    Logger::Log("Compass shader loading complete!");

    Logger::Log("Loading compass texture...");
    compassTexture = imageManager->AddImage("images/DirectionDial.png");
    if (compassTexture == 0)
    {
        return false;
    }

    Logger::Log("Compass texture image loading done!");

    // Load the ship map and compass.
    InitializeCompasses();
    InitializeShipMaps();
    
    // Load the position and rotation text.
    xSentence = fontManager->CreateNewSentence();
    ySentence = fontManager->CreateNewSentence();
    zSentence = fontManager->CreateNewSentence();

    xPosSentence = fontManager->CreateNewSentence();
    yPosSentence = fontManager->CreateNewSentence();
    zPosSentence = fontManager->CreateNewSentence();

    return true;
}

// Loads in a compass indicator into the currently-active vertex buffer.
void ShipHud::LoadCompassIndicator(universalVertices& vertices, vmath::vec3 colorMax)
{
    vertices.AddColorTextureVertex(vmath::vec3(-compassSize / 2, -compassSize / 2, 0), vmath::vec3(0, 0, 0), vmath::vec2(0, 1));
    vertices.AddColorTextureVertex(vmath::vec3(-compassSize / 2, compassSize / 2, 0), colorMax, vmath::vec2(0, 0));
    vertices.AddColorTextureVertex(vmath::vec3(-compassSize / 2 + compassSize, -compassSize / 2, 0), vmath::vec3(0, 0, 0), vmath::vec2(1, 1));

    vertices.AddColorTextureVertex(vmath::vec3(compassSize / 2, -compassSize / 2, 0), vmath::vec3(0, 0, 0), vmath::vec2(1, 1));
    vertices.AddColorTextureVertex(vmath::vec3(-compassSize / 2, compassSize / 2, 0), colorMax, vmath::vec2(0, 0));
    vertices.AddColorTextureVertex(vmath::vec3(compassSize / 2, compassSize / 2, 0), colorMax, vmath::vec2(1, 0));
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

// Updates the ship position text and map location.
void ShipHud::UpdateShipPosition(vmath::vec3& shipOrientation, vmath::vec3& shipPosition)
{
    std::stringstream textOutputStream;
    textOutputStream.precision(2);
    textOutputStream << std::fixed;

    textOutputStream << "X: " << shipPosition[0];
    fontManager->UpdateSentence(xPosSentence, textOutputStream.str(), 20, shipOrientation[0] > 0 ? vmath::vec3(0.0f, 1.0f, 0.0f) : vmath::vec3(1.0f, 0.0f, 0.0f));

    textOutputStream.str("");
    textOutputStream << "Y: " << shipPosition[1];
    fontManager->UpdateSentence(yPosSentence, textOutputStream.str(), 20, shipOrientation[1] > 0 ? vmath::vec3(0.0f, 1.0f, 0.0f) : vmath::vec3(1.0f, 0.0f, 0.0f));

    textOutputStream.str("");
    textOutputStream << "Z: " << shipPosition[2];
    fontManager->UpdateSentence(zPosSentence, textOutputStream.str(), 20, shipOrientation[2] > 0 ? vmath::vec3(0.0f, 1.0f, 0.0f) : vmath::vec3(1.0f, 0.0f, 0.0f));

    // (0, 0, 0) is the center of the sun.
    // Note that for every displayable unit (large scale), there are 10 units of traversal, and that the max view area is 30x30
    // Also note that the map has a border that needs to be accounted for.
 
    vmath::vec3 transformedCoordinates = (shipPosition / (10.0f * 15.0f * 2.0f)) + vmath::vec3(0.5f, 0.5f, 0.5f);

    // Now account for the map border.
    transformedCoordinates *= (1.0f - 2 * mapBorderWidth);
    transformedCoordinates += mapBorderWidth;
    indicatorPosSize = vmath::vec3(transformedCoordinates[0], 1.0f - transformedCoordinates[1], positionIndicatorSize);

    verticalIndicatorPosSize = vmath::vec3(0.5f, 1.0f - transformedCoordinates[2], positionIndicatorSize);
}

// Renders the HUD of the ship.
void ShipHud::RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock)
{
    // Bind in the texture and vertices we're using.
    glUseProgram(compassTextureProgram);
    glBindVertexArray(compassVao);
    
    glUniform1i(glGetUniformLocation(compassTextureProgram, "imageTex"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, compassTexture);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, perspectiveMatrix);

    // Draw the XY compass.
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, xyCompassMatrix * vmath::rotate(xyzCompassRotations[0], vmath::vec3(0, 0, -1)));
    glDrawArrays(GL_TRIANGLES, xyCompassOffset, compassVertexCount);

    // YZ compass
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, yzCompassMatrix * vmath::rotate(xyzCompassRotations[1], vmath::vec3(0, 0, -1)));
    glDrawArrays(GL_TRIANGLES, yzCompassOffset, compassVertexCount);

    // ZX compass
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, zxCompassMatrix * vmath::rotate(xyzCompassRotations[2], vmath::vec3(0, 0, -1)));
    glDrawArrays(GL_TRIANGLES, zxCompassOffset, compassVertexCount);

    // Draw the map
    glUseProgram(shipMapShaderProgram);
    glBindVertexArray(shipMapVao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shipMapTexture);
    glUniform1i(shipMapTexLocation, 0);

    glUniformMatrix4fv(shipMapProjLocation, 1, GL_FALSE, perspectiveMatrix);
    glUniformMatrix4fv(shipMapMvLocation, 1, GL_FALSE, shipMapMatrix);

    glUniform2f(indicatorPosLocation, indicatorPosSize[0], indicatorPosSize[1]);
    glUniform2f(indicatorSizeLocation, indicatorPosSize[2], indicatorPosSize[2]);
    glUniform3f(indicatorColorLocation, indicatorColor[0], indicatorColor[1], indicatorColor[2]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, shipMapVertexCount);

    // Draw the vertical map.
    glBindVertexArray(verticalMapVao);
    glUniformMatrix4fv(shipMapMvLocation, 1, GL_FALSE, verticalMapMatrix);

    glUniform2f(indicatorPosLocation, verticalIndicatorPosSize[0], verticalIndicatorPosSize[1]);
    glUniform2f(indicatorSizeLocation, (1.0f - 2.0f * mapBorderWidth) / 2.0f, verticalIndicatorPosSize[2]);
    glUniform3f(indicatorColorLocation, verticalIndicatorColor[0], verticalIndicatorColor[1], verticalIndicatorColor[2]);

    // TODO shader cross-coupling removal, if ever needed.
    glDrawArrays(GL_TRIANGLE_FAN, 0, shipMapVertexCount);

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
    glDeleteBuffers(1, &compassPositionBuffer);
    glDeleteBuffers(1, &compassColorBuffer);
    glDeleteBuffers(1, &compassUvBuffer);

    glDeleteVertexArrays(1, &shipMapVao);
    glDeleteBuffers(1, &shipMapPositionBuffer);
    glDeleteBuffers(1, &shipMapColorBuffer);
    glDeleteBuffers(1, &shipMapUvBuffer);

    glDeleteVertexArrays(1, &verticalMapVao);
    glDeleteBuffers(1, &verticalMapPositionBuffer);
    glDeleteBuffers(1, &verticalMapColorBuffer);
    glDeleteBuffers(1, &verticalMapUvBuffer);
}
