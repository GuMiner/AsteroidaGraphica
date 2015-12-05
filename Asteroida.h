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

public:
    Asteroida();
    bool Initialize(ShaderManager& shaderManager);
    void Render(vmath::mat4& projectionMatrix);
    ~Asteroida();
};

