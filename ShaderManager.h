#pragma once
#include <vector>
#include <glbinding/gl/gl.h>

class ShaderManager
{
    bool CreateShader(gl::GLenum shaderType, const char *shaderSource, gl::GLuint *shaderId);

    std::vector<gl::GLuint> shaderPrograms;
public:
    ShaderManager();
    bool CreateShaderProgram(const char *rootName, gl::GLuint *programId);
    void DeleteShaderProgram(gl::GLuint programId);
    ~ShaderManager();
};

