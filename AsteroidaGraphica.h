#pragma once

#include <memory>
#include <thread>
#include "ImageManager.h"
#include "Logger.hpp"
#include "Physica.h"
#include "ShaderManager.h"
#include "ShipHud.h"
#include "Version.h"
#include "vmath.hpp"

// Handles startup and the base graphics rendering loop.
class AsteroidaGraphica
{
    ShipHud shipHud;

    vmath::mat4 perspectiveMatrix;
    GLuint flatShaderProgram;
    
    // TEST OBJECTS
    vmath::mat4 lookAtMatrix;
    GLuint vao;
    GLuint pointBuffer;
    GLsizei vertexCount;
    GLint mv_location, proj_location;

    ImageManager imageManager;
    ShaderManager shaderManager;
    Physica physicsManager;
    sf::Thread physicaThread;

    Version::Status LoadFirstTimeGraphics();
    void LogGraphicsSettings();
    void UpdatePerspective(unsigned int width, unsigned int height);
public:
    // Held here as a common storage place.
    static std::unique_ptr<Logger> Log;
    static Version Version;

    AsteroidaGraphica();
    Version::Status Initialize();
    Version::Status Run();
    void Deinitialize();
};

