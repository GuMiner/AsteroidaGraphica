#pragma once

#include <SFML\System.hpp>
#include <GL/glew.h>
#include "Vertex.h"
#include "vmath.hpp"

class ShipHud
{
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
    GLint projLocation;
    GLint mvLocation;

    // Holds the compass rotations in degrees.
    vmath::vec3 xyzCompassRotations;

    void LoadCompassIndicator(colorTextureVertex *pVertices, GLsizei offset, vmath::vec3 colorMax);
public:
    ShipHud();
    void Initialize(GLuint compassTexture, GLint projLocation, GLint mvLocation);
    void UpdateCompassRotations(vmath::vec3& compassRotations);
    void RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock);

    ~ShipHud();
};

