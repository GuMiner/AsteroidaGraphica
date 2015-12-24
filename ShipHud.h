#pragma once
#include <glbinding/gl/gl.h>
#include <SFML\System.hpp>
#include "ImageManager.h"
#include "FontManager.h"
#include "Vertex.h"
#include "vmath.hpp"

class ShipHud
{
    // Compass data
    float compassSize;
    gl::GLsizei compassVertexCount;

    gl::GLuint xyCompassOffset;
    gl::GLuint yzCompassOffset;
    gl::GLuint zxCompassOffset;
    vmath::mat4 xyCompassMatrix;
    vmath::mat4 yzCompassMatrix;
    vmath::mat4 zxCompassMatrix;

    gl::GLuint compassTextureProgram;
    gl::GLint projLocation;
    gl::GLint mvLocation;
    gl::GLuint compassTexLocation;

    gl::GLuint compassVao;
    gl::GLuint compassPositionBuffer;
    gl::GLuint compassColorBuffer;
    gl::GLuint compassUvBuffer;
    gl::GLuint compassTexture;

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

    gl::GLsizei shipMapVertexCount; // Also the vertical map vertex count.

    gl::GLuint shipMapShaderProgram;
    gl::GLint shipMapProjLocation;
    gl::GLint shipMapMvLocation;
    gl::GLint shipMapTexLocation;
    gl::GLint indicatorSizeLocation;
    gl::GLint indicatorPosLocation;
    gl::GLint indicatorColorLocation;

    gl::GLuint shipMapTexture;

    gl::GLuint shipMapVao;
    gl::GLuint shipMapPositionBuffer;
    gl::GLuint shipMapColorBuffer;
    gl::GLuint shipMapUvBuffer;

    // The vertical map indicator (which is just the ship map indicator, but squashed)
    vmath::mat4 verticalMapMatrix;

    gl::GLuint verticalMapVao;
    gl::GLuint verticalMapPositionBuffer;
    gl::GLuint verticalMapColorBuffer;
    gl::GLuint verticalMapUvBuffer;

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

