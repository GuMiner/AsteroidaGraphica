#pragma once
#include <SFML\System.hpp>
#include <glbinding/gl/gl.h>
#include "ShaderManager.h"
#include "vmath.hpp"

using namespace gl;

// Manages the planets visible that the player can navigate to.
//  Also holds the inner and outer asteroid force fields.
class Planetia
{
	GLuint fieldShaderProgram;
	GLuint projLocation;
	GLuint phaseLocation;

	GLuint innerOuterShieldsVao;
	GLuint positionBuffer;
	GLuint indicesBuffer;
	GLsizei indiciesCount;

	sf::Clock clock;
public:
	Planetia();
	bool Initialize(ShaderManager& shaderManager);
	void Render(vmath::mat4& projectionMatrix);
	~Planetia();
};

