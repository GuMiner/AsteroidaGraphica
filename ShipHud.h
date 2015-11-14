#pragma once

#include <SFML\System.hpp>
#include <GL/glew.h>
#include "Vertex.h"

class ShipHud
{
    float compassSize;
    float compassDepth;
    GLsizei compassVertexCount;

    float xyCompassX;
    float xyCompassY;
    GLint xyCompassOffset;
    
    float yzCompassX;
    float yzCompassY;
    GLint yzCompassOffset;

    float zxCompassX;
    float zxCompassY;
    GLint zxCompassOffset;

    GLuint compassVao;
    GLuint compassVertexBuffer;

    GLuint compassTexture;
    GLint projLocation;

    void LoadCompassIndicator(colorTextureVertex *pVertices, GLsizei offset, float compassX, float compassY, vmath::vec3 colorMax);
public:
    ShipHud();
    void Initialize(GLuint compassTexture, GLint projLocation);
    void RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock);

    ~ShipHud();
};

