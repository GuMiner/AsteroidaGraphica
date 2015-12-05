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

    // TEST CODE TEST CODE
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Setup of vertex transfer (note we're using the "vertex" object in CodeGell)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Send some random data to the GPU
    // This was a reload operation, changes were performed that must be flushed to the GPU.
    colorVertex *pVertices = new colorVertex[9];
    pVertices[0].Set(0, 0, 0, 1, 1, 1);
    pVertices[1].Set(0, 5, 0, 0, 1, 0);
    pVertices[2].Set(5, 0, 0, 1, 0, 0);

    pVertices[3].Set(0, 5, 0, 0, 1, 0);
    pVertices[4].Set(0, 0, 5, 0, 0, 1);
    pVertices[5].Set(0, 0, 0, 1, 1, 1);

    pVertices[6].Set(5, 0, 0, 1, 0, 0);
    pVertices[7].Set(0, 0, 5, 0, 0, 1);
    pVertices[8].Set(0, 0, 0, 1, 1, 1);

    vertexCount = 9;
    colorVertex::TransferToOpenGl(pVertices, vertexCount);

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
