#include "Geometry.h"
#include "Logger.h"
#include "Vertex.h"
#include "Planetia.h"
#include "WorldGenerationConfig.h"

Planetia::Planetia()
{
	clock.restart();
}

// Sets up planets and the force field.
bool Planetia::Initialize(ShaderManager& shaderManager)
{
	// Create shader and GPU data
	Logger::Log("Force field shader creation...");
	if (!shaderManager.CreateShaderProgram("fieldRender", &fieldShaderProgram))
	{
		return false;
	}

	projLocation = glGetUniformLocation(fieldShaderProgram, "projMatrix");
	phaseLocation = glGetUniformLocation(fieldShaderProgram, "phase");

	glGenVertexArrays(1, &innerOuterShieldsVao);
	glBindVertexArray(innerOuterShieldsVao);
	glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &indicesBuffer);

	// Create the force fields.
	Geometry geometry;
	universalVertices vertices;
	geometry.GenerateForceField(vertices, WorldGenerationConfig::AsteroidTorusMinDistance);
	geometry.GenerateForceField(vertices, WorldGenerationConfig::AsteroidTorusMinDistance + WorldGenerationConfig::AsteroidTorusRadius);

	vertices.TransferPositionToOpenGl(positionBuffer);
	vertices.TransferIndicesToOpenGl(indicesBuffer);
	indiciesCount = vertices.indices.size();

	return true;
}

// Renders the planets and force fields.
void Planetia::Render(vmath::mat4& projectionMatrix)
{
	glUseProgram(fieldShaderProgram);
	glBindVertexArray(innerOuterShieldsVao);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
	glUniform1f(phaseLocation, clock.getElapsedTime().asSeconds());

	glDrawElements(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, nullptr);
}

Planetia::~Planetia()
{
	glDeleteVertexArrays(1, &innerOuterShieldsVao);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &indicesBuffer);
}
