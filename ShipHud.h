#pragma once

#include <SFML\System.hpp>
#include <GL/glew.h>

class ShipHud
{
    GLuint compassTexture;
    GLint mvLocation, projLocation;

    // Vertex buffer for displaying 
    GLuint vao;
    GLuint vertexBuffer;
    GLsizei vertexCount;

public:
    ShipHud();
    void Initialize(GLuint compassTexture, GLint mvLocation, GLint projLocation);
    void RenderHud(vmath::mat4& perspectiveMatrix, sf::Clock& clock);

    ~ShipHud();
};

