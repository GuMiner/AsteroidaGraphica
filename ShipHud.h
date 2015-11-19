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
    vmath::mat4 xyCompassTranslation;
    vmath::mat4 yzCompassTranslation;
    vmath::mat4 zxCompassTranslation;

    GLuint compassVao;
    GLuint compassVertexBuffer;
    GLuint compassTexture;

    // Text data
    GLuint textVao;
    GLuint textVertexBuffer;
    GLsizei textVertexCount;
    vmath::mat4 textTranslation;

    // General shader and UI data
    GLint projLocation;
    GLint mvLocation;

    FontManager* fontManager;

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

