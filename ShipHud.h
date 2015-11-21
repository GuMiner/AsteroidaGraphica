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

    // Text data
    vmath::mat4 textScale;

    // Single sentence data.
    int sentenceId;
    vmath::mat4 textTranslation;

    // General shader and UI data
    GLint projLocation;
    GLint mvLocation;

    FontManager* fontManager;
    float hudDepth;

    // Holds the compass rotations in degrees.
    vmath::vec3 xyzCompassRotations;

    void LoadCompassIndicator(colorTextureVertex *pVertices, GLsizei offset, vmath::vec3 colorMax);
    void ReloadRotationalText();
public:
    ShipHud();
    void Initialize(FontManager* fontManager, GLuint compassTexture, GLint projLocation, GLint mvLocation);
    void UpdateCompassRotations(vmath::vec3& compassRotations);
    void RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock);

    ~ShipHud();
};

