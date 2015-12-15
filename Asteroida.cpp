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

    // TODO there should be some nice way of randomly generating asteroids with consumption stats.
    Geometry geometry;
    std::vector<colorBarycentricVertex> vertices = geometry.GenerateLargeAsteroid();
    vertexCount = vertices.size();
    colorBarycentricVertex *pVertices = new colorBarycentricVertex[vertexCount];
    for (int i = 0; i < vertexCount; i++)
    {
        // TODO use a better copy mechanism.
        pVertices[i] = vertices[i];
    }

    colorBarycentricVertex::TransferToOpenGl(pVertices, vertexCount);
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
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
