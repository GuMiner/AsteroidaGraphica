#pragma once
#include <SFML\System.hpp>
#include <glbinding/gl/gl.h>
#include "ShaderManager.h"
#include "Vertex.h"
#include "vmath.hpp"

using namespace gl;

// Manages the sun.
class Stellaria
{
	GLuint sunShaderProgram;
	GLuint projLocation;

	GLuint vao;
	GLuint positionBuffer;
	GLuint barycentricBuffer;
	GLuint indicesBuffer;

	universalVertices sunVertices;

	sf::Mutex vibrationLock;
	universalVertices tempPositionVertices;
public:
	Stellaria();
	bool Initialize(ShaderManager& shaderManager);
	void Update();
	void Render(vmath::mat4& projectionMatrix);
	~Stellaria();
};

