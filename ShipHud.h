#pragma once
#include <SFML\System.hpp>
#include <GL/glew.h>
#include "ImageManager.h"
#include "FontManager.h"
#include "Vertex.h"
#include "vmath.hpp"

class ShipHud
{
    // Compass data
    float compassSize;
    GLsizei compassVertexCount;

    GLuint xyCompassOffset;
    GLuint yzCompassOffset;
    GLuint zxCompassOffset;
    vmath::mat4 xyCompassMatrix;
    vmath::mat4 yzCompassMatrix;
    vmath::mat4 zxCompassMatrix;

    GLuint compassTextureProgram;
    GLint projLocation;
    GLint mvLocation;
    GLuint compassTexLocation;

    GLuint compassVao;
    GLuint compassPositionBuffer;
    GLuint compassColorBuffer;
    GLuint compassUvBuffer;
    GLuint compassTexture;

    // XYZ compass text information.
    vmath::mat4 textScale;

    int xSentence;
    vmath::mat4 xTextMatrix;
    
    int ySentence;
    vmath::mat4 yTextMatrix;
    
    int zSentence;
    vmath::mat4 zTextMatrix;

    int xPosSentence;
    vmath::mat4 xPosTextMatrix;

    int yPosSentence;
    vmath::mat4 yPosTextMatrix;

    int zPosSentence;
    vmath::mat4 zPosTextMatrix;
    
    FontManager* fontManager;
    float hudDepth;

    // Holds the compass rotations in degrees.
    vmath::vec3 xyzCompassRotations;

    // Ship Map data
    float mapSize;
    float positionIndicatorSize;
    vmath::mat4 shipMapMatrix;

    GLsizei shipMapVertexCount; // Also the vertical map vertex count.

    GLuint shipMapShaderProgram;
    GLint shipMapProjLocation;
    GLint shipMapMvLocation;
    GLint shipMapTexLocation;
    GLint indicatorSizeLocation;
    GLint indicatorPosLocation;
    GLint indicatorColorLocation;

    GLuint shipMapTexture;

    GLuint shipMapVao;
    GLuint shipMapPositionBuffer;
    GLuint shipMapColorBuffer;
    GLuint shipMapUvBuffer;

    // The vertical map indicator (which is just the ship map indicator, but squashed)
    vmath::mat4 verticalMapMatrix;

    GLuint verticalMapVao;
    GLuint verticalMapPositionBuffer;
    GLuint verticalMapColorBuffer;
    GLuint verticalMapUvBuffer;

    // Ship Map information
    float mapBorderWidth;
    vmath::vec3 indicatorPosSize;
    vmath::vec3 indicatorColor;

    vmath::vec3 verticalIndicatorPosSize;
    vmath::vec3 verticalIndicatorColor;

    void InitializeCompasses();
    void InitializeShipMaps();
    void LoadCompassIndicator(universalVertices& vertices, vmath::vec3 colorMax);
public:
    ShipHud();
    bool Initialize(ShaderManager* shaderManager, FontManager* fontManager, ImageManager* imageManager);
    void UpdateCompassRotations(vmath::vec3& compassRotations);
    void UpdateShipPosition(vmath::vec3& shipOrientation, vmath::vec3& shipPosition);
    void RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock);

    ~ShipHud();
};

