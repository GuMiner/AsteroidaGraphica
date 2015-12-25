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

    // Asteroid random generation with resource statistics.
    vertexCount = 0;
    smallAsteroidVertexCounts = new GLuint[ConfigManager::SmallAsteroidTypes];
    mediumAsteroidVertexCounts = new GLuint[ConfigManager::MediumAsteroidTypes];
    largeAsteroidVertexCounts = new GLuint[ConfigManager::LargeAsteroidTypes];

    smallAsteroidOffsets = new GLuint[ConfigManager::SmallAsteroidTypes];
    mediumAsteroidOffsets = new GLuint[ConfigManager::MediumAsteroidTypes];
    largeAsteroidOffsets = new GLuint[ConfigManager::LargeAsteroidTypes];
    
    Geometry geometry;
	universalVertices allAsteroids;
    
    Logger::Log("Generating small asteroid types...");
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

    // TODO randomly set asteroid data so we know *which* asteroid type is at which index
	Logger::Log("Forming asteroids into a vertex buffer for bulk drawing...");
	glGenBuffers(1, &indirectDrawBuffer);
    DrawArraysIndirectCommand* draws = new DrawArraysIndirectCommand[ConfigManager::AsteroidCount];
    for (int i = 0; i < ConfigManager::AsteroidCount; i++)
    {
		if (i < ConfigManager::SmallAsteroidTypes)
        {
            // Small asteroid
            draws[i].Set(smallAsteroidVertexCounts[i], 1, smallAsteroidOffsets[i], i);
        }
        else if (i < ConfigManager::MediumAsteroidTypes + ConfigManager::SmallAsteroidTypes)
        {
            // Medium asteroid
            draws[i].Set(mediumAsteroidVertexCounts[i - 200], 1, mediumAsteroidOffsets[i - 200], i);
        }
        else
        {
            // Large asteroid
            draws[i].Set(largeAsteroidVertexCounts[i - 400], 1, largeAsteroidOffsets[i - 400], i);
        }
    }

	Logger::Log("Adding ID field for asteroids");
	for (unsigned int i = 0; i < (unsigned int)ConfigManager::AsteroidCount; i++)
	{
		for (unsigned int j = 0; j < draws[i].vertexCount; j++)
		{
			allAsteroids.ids.push_back(i);
		}
	}

	// TODO delete this step when I move away from indirect drawing.
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectDrawBuffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, ConfigManager::AsteroidCount * sizeof(DrawArraysIndirectCommand), draws, GL_STATIC_DRAW);
	delete[] draws;


	Logger::Log("Sending asteroid data to the GPU...");
	universalVertices::TransferToOpenGl(allAsteroids, positionBuffer, 0, barycentricBuffer, 0, idBuffer, 0);


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
    
    glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr, ConfigManager::AsteroidCount, 0);
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

    glDeleteBuffers(1, &indirectDrawBuffer);

    glDeleteTextures(1, &asteroidPositionTexture);


}
