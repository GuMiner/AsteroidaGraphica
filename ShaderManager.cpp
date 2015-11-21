#include "Logger.h"
#include "StringUtils.hpp"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
    : shaderPrograms()
{
}

// Creates and compiles a new shader of the specified type; returns true on success.
bool ShaderManager::CreateShader(GLenum shaderType, const char *shaderSource, GLuint *shaderId)
{
    GLint compileStatus;

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
        char buffer[1024];
        GLint len;
        std::stringstream logStream;
        
        glGetShaderInfoLog(shader, 1024, &len, buffer);
        logStream << "Error: " << glewGetErrorString(glGetError()) << " " << buffer;
        Logger::LogError(logStream.str().c_str());
        return false;
    }

    *shaderId = shader;
    return true;
}

// Creates a shader program and adds it to the list of programs that will be deleted at the end of program operation
bool ShaderManager::CreateShaderProgram(const char *rootName, GLuint *programId)
{
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;

    std::string vsShader, fsShader;
    std::stringstream logStream;
    std::stringstream vsFilenameStream, fsFilenameStream;
    vsFilenameStream << "shaders/" << rootName << ".vs";
    fsFilenameStream << "shaders/" << rootName << ".fs";

    if (!StringUtils::LoadStringFromFile(vsFilenameStream.str().c_str(), vsShader))
    {
        logStream << "Could not load vertex shader: " << vsShader << "!";
        Logger::LogError(logStream.str().c_str());
        return false;
    }

    if (!StringUtils::LoadStringFromFile(fsFilenameStream.str().c_str(), fsShader))
    {
        logStream << "Could not load fragment shader: " << fsShader << "!";
        Logger::LogError(logStream.str().c_str());
        return false;
    }

    bool result = CreateShader(GL_VERTEX_SHADER, vsShader.c_str(), &vertexShader);
    if (!result)
    {
        return false;
    }

    result = CreateShader(GL_FRAGMENT_SHADER, fsShader.c_str(), &fragmentShader);
    if (!result)
    {
        return false;
    }

    // Create the program
    GLint compileStatus;
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &compileStatus);
    if (!compileStatus)
    {
        char buffer[1024];
        GLint len;
        glGetProgramInfoLog(program, 1024, &len, buffer);
        logStream << glewGetErrorString(glGetError()) << " " << buffer;
        Logger::LogError(logStream.str().c_str());
        return false;
    }

    // These are auto-deleted when the program is deleted
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    *programId = program;
    shaderPrograms.push_back(program);
    return true;
}

// Deletes the specified shader program, removing it from the list of known programs.
void ShaderManager::DeleteShaderProgram(GLuint program)
{
    glDeleteProgram(program);
    for (unsigned int i = 0; i < shaderPrograms.size(); i++)
    {
        if (shaderPrograms[i] == program)
        {
            shaderPrograms.erase(shaderPrograms.begin() + i);
            break;
        }
    }
}

ShaderManager::~ShaderManager()
{
    for (unsigned int i = 0; i < shaderPrograms.size(); i++)
    {
        glDeleteProgram(shaderPrograms[i]);
    }

    shaderPrograms.clear();
}