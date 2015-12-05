#include "Asteroida.h"
#include "Logger.h"
#include "Vertex.h"

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
    vertexCount = 12;
    colorBarycentricVertex *pVertices = new colorBarycentricVertex[vertexCount];
    pVertices[0].Set(0, 0, 0, 1, 1, 1, 0, 0, 1);
    pVertices[1].Set(0, 5, 0, 0, 1, 0, 0, 1, 0);
    pVertices[2].Set(5, 0, 0, 1, 0, 0, 1, 0, 0);

    pVertices[3].Set(0, 5, 0, 0, 1, 0, 0, 0, 1);
    pVertices[4].Set(0, 0, 5, 0, 0, 1, 0, 1, 0);
    pVertices[5].Set(0, 0, 0, 1, 1, 1, 1, 0, 0);

    pVertices[6].Set(5, 0, 0, 1, 0, 0, 0, 0, 1);
    pVertices[7].Set(0, 0, 5, 0, 0, 1, 0, 1, 0);
    pVertices[8].Set(0, 0, 0, 1, 1, 1, 1, 0, 0);

    pVertices[9].Set(5, 0, 0, 1, 0, 0, 0, 0, 1);
    pVertices[10].Set(0, 0, 5, 0, 0, 1, 0, 1, 0);
    pVertices[11].Set(0, 5, 0, 0, 1, 0, 1, 0, 0);

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
                vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(i*separation, j*separation, k*separation));
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
