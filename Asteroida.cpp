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
    smallAsteroidVertexCounts = new GLuint[ConfigManager::SmallAsteroidTypes];
    mediumAsteroidVertexCounts = new GLuint[ConfigManager::MediumAsteroidTypes];
    largeAsteroidVertexCounts = new GLuint[ConfigManager::LargeAsteroidTypes];
    
    Geometry geometry;
    GLuint totalVertexCount = 0;
    std::vector<std::vector<barycentricVertex>> smallAsteroids;
    std::vector<std::vector<barycentricVertex>> mediumAsteroids;
    std::vector<std::vector<barycentricVertex>> largeAsteroids;
    
    for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
    {
        smallAsteroids.push_back(geometry.GenerateSmallAsteroid());
    }

    for (int i = 0; i < ConfigManager::MediumAsteroidTypes; i++)
    {
        mediumAsteroids.push_back(geometry.GenerateMediumAsteroid());
    }

    for (int i = 0; i < ConfigManager::LargeAsteroidTypes; i++)
    {
        largeAsteroids.push_back(geometry.GenerateLargeAsteroid());
    }

    glGenBuffers(1, &asteroidIdBuffer);

    
    std::vector<barycentricVertex> vertices = geometry.GenerateLargeAsteroid();
    vertexCount = vertices.size();
    barycentricVertex *pVertices = new barycentricVertex[vertexCount];
    for (int i = 0; i < vertexCount; i++)
    {
        // TODO use a better copy mechanism.
        pVertices[i] = vertices[i];
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

    // Let's add a 5x5x5 grid of these things to help with debugging.
    float separation = 7;
    int scale = 4;
    for (int i = 0; i < scale; i++)
    {
        for (int j = 0; j < scale; j++)
        {
            for (int k = 0; k < scale; k++)
            {
                vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(i*separation, j*separation, k*separation)) * vmath::translate(-5, -5, -5);
                glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
                glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            }
        }
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
