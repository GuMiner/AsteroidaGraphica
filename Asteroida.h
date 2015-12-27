#pragma once
#include <SFML\System.hpp>
#include <glbinding/gl/gl.h>
#include "ShaderManager.h"
#include "vmath.hpp"

using namespace gl;

// Manages the asteroids that are part of this game.
class Asteroida
{
	GLuint asteroidShaderProgram;
    GLuint mvLocation, projLocation;

    GLuint vao;
    GLuint positionBuffer;
	GLuint barycentricBuffer;
	GLuint idBuffer;
	GLuint indicesBuffer;
	    
	GLuint asteroidPositionTexture;
	GLuint asteroidColorTexture;

	// Asteroid data
	std::vector<vmath::vec3> colors;
	std::vector<float> masses;

	GLuint asteroidIndexCount;

    // Note that asteroids are stored in the VBO in small-medium-large order.
    GLuint* smallAsteroidVertexCounts;
    GLuint* mediumAsteroidVertexCounts;
    GLuint* largeAsteroidVertexCounts;
    GLuint* smallAsteroidOffsets;
    GLuint* mediumAsteroidOffsets;
    GLuint* largeAsteroidOffsets;

public:
	// Physics data used for updating within Physica.
	sf::Mutex updateMutex;
	bool updatedAsteroidPosition;
	std::vector<vmath::vec4> positions;
	std::vector<vmath::vec3> velocities;

    Asteroida();
    bool Initialize(ShaderManager& shaderManager);
    void Render(vmath::mat4& projectionMatrix);
    ~Asteroida();
};

