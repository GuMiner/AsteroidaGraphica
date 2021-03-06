#include "Stellaria.h"
#include "ConfigManager.h"
#include "Geometry.h"
#include "Logger.h"

Stellaria::Stellaria()
{
}

bool Stellaria::Initialize(ShaderManager & shaderManager)
{
	// Shaders
	Logger::Log("Stellaria shader creation...");
	if (!shaderManager.CreateShaderProgram("sunRender", &sunShaderProgram))
	{
		return false;
	}
	Logger::Log("Stellaria shader creation successful!");

	projLocation = glGetUniformLocation(sunShaderProgram, "proj_matrix");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &barycentricBuffer);
	glGenBuffers(1, &indicesBuffer);

	Logger::Log("Generating the sun...");
	Geometry geometry;
	GLuint vertexCount = geometry.GenerateSun(sunVertices);
	
	Logger::Log("Sending the sun to the GPU...");
	sunVertices.TransferPositionToOpenGl(positionBuffer);
	sunVertices.TransferBarycentricsToOpenGl(barycentricBuffer);
	sunVertices.TransferIndicesToOpenGl(indicesBuffer);
	return true;
}

// TODO: Updates the sun.
void Stellaria::Update()
{
	// Make the sun seem to randomly 'vibrate'

	vibrationLock.lock();
	tempPositionVertices.positions.clear();
	for (unsigned int i = 0; i < sunVertices.positions.size(); i++)
	{
		tempPositionVertices.positions.push_back(sunVertices.positions[i] + Constants::Rand(0.20f));
	}
	vibrationLock.unlock();
}

// Renders the sun.
void Stellaria::Render(vmath::mat4& projectionMatrix)
{
	glUseProgram(sunShaderProgram);
	glBindVertexArray(vao);

	vibrationLock.lock();
	tempPositionVertices.TransferPositionToOpenGl(positionBuffer);
	vibrationLock.unlock();

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
	glDrawElements(GL_TRIANGLES, sunVertices.indices.size(), GL_UNSIGNED_INT, nullptr);
}

// Deletes all the elements we needed to create for the sun.
Stellaria::~Stellaria()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &barycentricBuffer);
	glDeleteBuffers(1, &indicesBuffer);
}
