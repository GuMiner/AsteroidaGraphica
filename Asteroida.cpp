#include "ConfigManager.h"
#include "Geometry.h"
#include "Logger.h"
#include "Vertex.h"
#include "Asteroida.h"

Asteroida::Asteroida()
{
}

bool Asteroida::Initialize(ShaderManager& shaderManager)
{
    // Shaders
    Logger::Log("Asteroida shader creation...");
    if (!shaderManager.CreateShaderProgram("asteroidRender", &asteroidShaderProgram))
    {
        return false;
    }
    Logger::Log("Asteroida shader creation successful!");

    mvLocation = glGetUniformLocation(asteroidShaderProgram, "mv_matrix");
    projLocation = glGetUniformLocation(asteroidShaderProgram, "proj_matrix");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &barycentricBuffer);
	glGenBuffers(1, &idBuffer);
	glGenBuffers(1, &indicesBuffer);

    // Asteroid random generation with resource statistics.
    smallAsteroidVertexCounts = new GLuint[ConfigManager::SmallAsteroidTypes];
    mediumAsteroidVertexCounts = new GLuint[ConfigManager::MediumAsteroidTypes];
    largeAsteroidVertexCounts = new GLuint[ConfigManager::LargeAsteroidTypes];

    smallAsteroidOffsets = new GLuint[ConfigManager::SmallAsteroidTypes];
    mediumAsteroidOffsets = new GLuint[ConfigManager::MediumAsteroidTypes];
    largeAsteroidOffsets = new GLuint[ConfigManager::LargeAsteroidTypes];
    
    Geometry geometry;
	universalVertices allAsteroids;
    
    Logger::Log("Generating small asteroid types...");
	GLuint vertexCount = 0;
    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
		smallAsteroidVertexCounts[i] = geometry.GenerateSmallAsteroid(allAsteroids);
        smallAsteroidOffsets[i] = vertexCount;
        vertexCount += smallAsteroidVertexCounts[i];
    }

    Logger::Log("Generating medium asteroid types...");
    for (int i = 0; i < ConfigManager::MediumAsteroidTypes; i++)
    {
		mediumAsteroidVertexCounts[i] = geometry.GenerateMediumAsteroid(allAsteroids);
		mediumAsteroidOffsets[i] = vertexCount;
		vertexCount += mediumAsteroidVertexCounts[i];
    }

    Logger::Log("Generating large asteroid types...");
    for (int i = 0; i < ConfigManager::LargeAsteroidTypes; i++)
    {
        largeAsteroidVertexCounts[i] = geometry.GenerateLargeAsteroid(allAsteroids);
        largeAsteroidOffsets[i] = vertexCount;
        vertexCount += largeAsteroidVertexCounts[i];
    }
    
    // TODO texture and indirect drawing setup.
	Logger::Log("Sending asteroid customization data to the GPU...");
    glGenTextures(1, &asteroidPositionTexture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, asteroidPositionTexture);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA, ConfigManager::AsteroidCount);
	
    // TODO Randomly position the asteroids in the appropriate zones.
    float separation = 2.0f;
    float* positions = new float[4 * ConfigManager::AsteroidCount];

    int fracAsteroidCount = (int)sqrt(ConfigManager::AsteroidCount);
    for (int i = 0; i < ConfigManager::AsteroidCount; i++)
    {
        int texelIndex = 4 * i;
        positions[texelIndex] = 1.0f;
        positions[texelIndex + 1] = (i / fracAsteroidCount) * separation;
        positions[texelIndex + 2] = (i % fracAsteroidCount) * separation;
        positions[texelIndex + 3] = 0.0f;
    }

    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, ConfigManager::AsteroidCount, GL_RGBA, GL_FLOAT, positions);
    delete[] positions;

	Logger::Log("Adding ID field for asteroids");
	for (unsigned int i = 0; i < (unsigned int)ConfigManager::SmallAsteroidTypes; i++)
	{
		for (unsigned int j = 0; j < smallAsteroidVertexCounts[i]; j++)
		{
			allAsteroids.ids.push_back(i);
		}
	}

	for (unsigned int i = 0; i < (unsigned int)ConfigManager::MediumAsteroidTypes; i++)
	{
		for (unsigned int j = 0; j < mediumAsteroidVertexCounts[i]; j++)
		{
			allAsteroids.ids.push_back(i + 200);
		}
	}

	for (unsigned int i = 0; i < (unsigned int)ConfigManager::LargeAsteroidTypes; i++)
	{
		for (unsigned int j = 0; j < largeAsteroidVertexCounts[i]; j++)
		{
			allAsteroids.ids.push_back(i + 400);
		}
	}

	Logger::Log("Sending asteroid data to the GPU...");
	universalVertices::TransferToOpenGl(allAsteroids, positionBuffer, 0, barycentricBuffer, 0, idBuffer, indicesBuffer);
	asteroidIndexCount = allAsteroids.indices.size();

    return true;
}

// Renders the asteroids with the given perspective/look-at projection matrix.
void Asteroida::Render(vmath::mat4& projectionMatrix)
{
    glUseProgram(asteroidShaderProgram);
    glBindVertexArray(vao);
	
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
    vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
    
	glDrawElements(GL_TRIANGLES, asteroidIndexCount, GL_UNSIGNED_INT, nullptr);
}

Asteroida::~Asteroida()
{
    delete[] smallAsteroidVertexCounts;
    delete[] mediumAsteroidVertexCounts;
    delete[] largeAsteroidVertexCounts;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &barycentricBuffer);
	glDeleteBuffers(1, &idBuffer);
	glDeleteBuffers(1, &indicesBuffer);

    glDeleteTextures(1, &asteroidPositionTexture);


}
