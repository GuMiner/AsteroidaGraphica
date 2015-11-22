#pragma once
#include <SFML\System.hpp>
#include <GL/glew.h>
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

    GLuint compassVao;
    GLuint compassVertexBuffer;
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

    // General shader and UI data
    GLint projLocation;
    GLint mvLocation;

    FontManager* fontManager;
    float hudDepth;

    // Holds the compass rotations in degrees.
    vmath::vec3 xyzCompassRotations;

    void LoadCompassIndicator(colorTextureVertex *pVertices, GLsizei offset, vmath::vec3 colorMax);
public:
    ShipHud();
    void Initialize(FontManager* fontManager, GLuint compassTexture, GLint projLocation, GLint mvLocation);
    void UpdateCompassRotations(vmath::vec3& compassRotations);
    void UpdateShipPositition(vmath::vec3& shipPosition);
    void RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock);

    ~ShipHud();
};

