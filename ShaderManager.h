#pragma once
#include <GL/glew.h>
#include <vector>

class ShaderManager
{
    bool CreateShader(GLenum shaderType, const char *shaderSource, GLuint *shaderId);

    std::vector<GLuint> shaderPrograms;
public:
    ShaderManager();
    bool CreateShaderProgram(const char *rootName, GLuint *programId);
    void DeleteShaderProgram(GLuint programId);
    ~ShaderManager();
};
