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
    glGenBuffers(1, &asteroidIdBuffer);

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

    return true;
}

// Renders the asteroids with the given perspective/look-at projection matrix.
void Asteroida::Render(vmath::mat4& projectionMatrix)
{
    // TEST CODE DRAWING
    glUseProgram(asteroidShaderProgram);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);

    float separation = 20.0f;
    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
        float angle = (float)i*2.0f*3.16159f / (float)ConfigManager::SmallAsteroidTypes;
        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(separation*cosf(angle), 0.0f, separation*sinf(angle)));
        glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
        glDrawArrays(GL_TRIANGLES, smallAsteroidOffsets[i], smallAsteroidVertexCounts[i]);
    }

    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
        float angle = (float)i*2.0f*3.16159f / (float)ConfigManager::SmallAsteroidTypes;
        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(separation*cosf(angle), 2.0f, separation*sinf(angle)));
        glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
        glDrawArrays(GL_TRIANGLES, mediumAsteroidOffsets[i], mediumAsteroidVertexCounts[i]);
    }

    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
        float angle = (float)i*2.0f*3.16159f / (float)ConfigManager::SmallAsteroidTypes;
        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(separation*cosf(angle), 4.0f, separation*sinf(angle)));
        glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
        glDrawArrays(GL_TRIANGLES, largeAsteroidOffsets[i], largeAsteroidVertexCounts[i]);
    }
}

Asteroida::~Asteroida()
{
    delete[] smallAsteroidVertexCounts;
    delete[] mediumAsteroidVertexCounts;
    delete[] largeAsteroidVertexCounts;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glDeleteBuffers(1, &asteroidIdBuffer);
}
