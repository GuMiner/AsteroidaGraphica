#pragma once
#include "ShaderManager.h"
#include "vmath.hpp"

// Manages the asteroids that are part of this game.
class Asteroida
{
    GLuint asteroidShaderProgram;
    GLuint mvLocation, projLocation;

    GLuint vao;
    GLuint vbo;
    GLsizei vertexCount;
    GLuint asteroidIdBuffer;

    // Note that asteroids are stored in the VBO in small-medium-large order.
    GLuint* smallAsteroidVertexCounts;
    GLuint* mediumAsteroidVertexCounts;
    GLuint* largeAsteroidVertexCounts;

public:
    Asteroida();
    bool Initialize(ShaderManager& shaderManager);
    void Render(vmath::mat4& projectionMatrix);
    ~Asteroida();
};

