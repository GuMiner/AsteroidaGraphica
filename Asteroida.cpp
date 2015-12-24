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

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Asteroid random generation with resource statistics.
    vertexCount = 0;
    smallAsteroidVertexCounts = new GLuint[ConfigManager::SmallAsteroidTypes];
    mediumAsteroidVertexCounts = new GLuint[ConfigManager::MediumAsteroidTypes];
    largeAsteroidVertexCounts = new GLuint[ConfigManager::LargeAsteroidTypes];

    smallAsteroidOffsets = new GLuint[ConfigManager::SmallAsteroidTypes];
    mediumAsteroidOffsets = new GLuint[ConfigManager::MediumAsteroidTypes];
    largeAsteroidOffsets = new GLuint[ConfigManager::LargeAsteroidTypes];
    
    Geometry geometry;
    std::vector<std::vector<barycentricVertex>> smallAsteroids;
    std::vector<std::vector<barycentricVertex>> mediumAsteroids;
    std::vector<std::vector<barycentricVertex>> largeAsteroids;
    
    Logger::Log("Generating small asteroid types...");
    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
        smallAsteroids.push_back(geometry.GenerateSmallAsteroid());
        smallAsteroidVertexCounts[i] = smallAsteroids[i].size();
        smallAsteroidOffsets[i] = vertexCount;
        vertexCount += smallAsteroidVertexCounts[i];
    }

    Logger::Log("Generating medium asteroid types...");
    for (int i = 0; i < ConfigManager::MediumAsteroidTypes; i++)
    {
        mediumAsteroids.push_back(geometry.GenerateMediumAsteroid());
        mediumAsteroidVertexCounts[i] = mediumAsteroids[i].size();
        mediumAsteroidOffsets[i] = vertexCount;
        vertexCount += mediumAsteroidVertexCounts[i];
    }

    Logger::Log("Generating large asteroid types...");
    for (int i = 0; i < ConfigManager::LargeAsteroidTypes; i++)
    {
        largeAsteroids.push_back(geometry.GenerateLargeAsteroid());
        largeAsteroidVertexCounts[i] = largeAsteroids[i].size();
        largeAsteroidOffsets[i] = vertexCount;
        vertexCount += largeAsteroidVertexCounts[i];
    }
    
    Logger::Log("Sending asteroid data to the GPU...");
    
    // Effectively duplicate our data and pack it, before deleting all of it, because it's now on the GPU.
    barycentricVertex *pVertices = new barycentricVertex[vertexCount];
    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
        std::copy(smallAsteroids[i].begin(), smallAsteroids[i].end(), &pVertices[smallAsteroidOffsets[i]]);
    }
    
    for (int i = 0; i < ConfigManager::MediumAsteroidTypes; i++)
    {
        std::copy(mediumAsteroids[i].begin(), mediumAsteroids[i].end(), &pVertices[mediumAsteroidOffsets[i]]);
    }

    for (int i = 0; i < ConfigManager::LargeAsteroidTypes; i++)
    {
        std::copy(largeAsteroids[i].begin(), largeAsteroids[i].end(), &pVertices[largeAsteroidOffsets[i]]);
    }

    barycentricVertex::TransferToOpenGl(pVertices, vertexCount);
    delete[] pVertices;

    // TODO texture and indirect drawing setup.
    glGenTextures(1, &asteroidPositionTexture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, asteroidPositionTexture);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA, ConfigManager::AsteroidCount);
	/*
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

    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, ConfigManager::AsteroidCount, GL_RGB, GL_FLOAT, positions);
    delete[] positions;*/

    // TODO randomly set asteroid data so we know *which* asteroid type is at which index
    glGenBuffers(1, &indirectDrawBuffer);
    DrawArraysIndirectCommand* draws = new DrawArraysIndirectCommand[ConfigManager::AsteroidCount];
    for (int i = 0; i < ConfigManager::AsteroidCount; i++)
    {
        int asteroidType = Constants::Rand(0, 3);
        if (asteroidType == 0)
        {
            // Small asteroid
            int archeType = Constants::Rand(0, ConfigManager::SmallAsteroidTypes);
            draws[i].Set(smallAsteroidVertexCounts[archeType], 1, smallAsteroidOffsets[archeType], i);
        }
        else if (asteroidType == 1)
        {
            // Medium asteroid
            int archeType = Constants::Rand(0, ConfigManager::MediumAsteroidTypes);
            draws[i].Set(mediumAsteroidVertexCounts[archeType], 1, mediumAsteroidOffsets[archeType], i);
        }
        else
        {
            // Large asteroid
            int archeType = Constants::Rand(0, ConfigManager::LargeAsteroidTypes);
            draws[i].Set(largeAsteroidVertexCounts[archeType], 1, largeAsteroidOffsets[archeType], i);
        }
    }

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectDrawBuffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, 2 * sizeof(DrawArraysIndirectCommand), draws, GL_STATIC_DRAW);
    delete[] draws;

    return true;
}

// Renders the asteroids with the given perspective/look-at projection matrix.
void Asteroida::Render(vmath::mat4& projectionMatrix)
{
    // TEST CODE DRAWING
    glUseProgram(asteroidShaderProgram);
    glBindVertexArray(vao);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
    vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
    
    glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr, 2, 0);
    
    /*
    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
        float angle = (float)i*2.0f*3.16159f / (float)ConfigManager::SmallAsteroidTypes;
        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(separation*cosf(angle), 0.0f, separation*sinf(angle)));
        glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
        glDrawArrays(GL_TRIANGLES, smallAsteroidOffsets[i], smallAsteroidVertexCounts[i]);
    }

    for (int i = 0; i < ConfigManager::MediumAsteroidTypes; i++)
    {
        float angle = (float)i*2.0f*3.16159f / (float)ConfigManager::SmallAsteroidTypes;
        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(separation*cosf(angle), 2.0f, separation*sinf(angle)));
        glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
        glDrawArrays(GL_TRIANGLES, mediumAsteroidOffsets[i], mediumAsteroidVertexCounts[i]);
    }
    
    float separation = 20.0f;
    for (int i = 0; i < ConfigManager::LargeAsteroidTypes; i++)
    {
        float angle = (float)i*2.0f*3.16159f / (float)ConfigManager::SmallAsteroidTypes;
        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(separation*cosf(angle), separation*sinf(angle), 3.0f));
        glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
        glDrawArraysInstancedBaseInstance(GL_TRIANGLES, largeAsteroidOffsets[i], largeAsteroidVertexCounts[i], 1, 20);
    }
    */
}

Asteroida::~Asteroida()
{
    delete[] smallAsteroidVertexCounts;
    delete[] mediumAsteroidVertexCounts;
    delete[] largeAsteroidVertexCounts;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glDeleteBuffers(1, &indirectDrawBuffer);

    glDeleteTextures(1, &asteroidPositionTexture);


}
