#pragma once

#include <memory>
#include <thread>
#include "ConfigManager.h"
#include "FontManager.h"
#include "ImageManager.h"
#include "Logger.h"
#include "MusicManager.h"
#include "Physica.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "ShipHud.h"
#include "Version.h"
#include "vmath.hpp"

// Handles startup and the base graphics rendering loop.
class AsteroidaGraphica
{
    ShipHud shipHud;

    vmath::mat4 perspectiveMatrix;
    GLuint flatShaderProgram;
    GLint mv_location, proj_location;
            
    // TEST OBJECTS
    vmath::mat4 lookAtMatrix;
    GLuint vao;
    GLuint pointBuffer;
    GLsizei vertexCount;

    // Managers
    ConfigManager configManager;
    ImageManager imageManager;
    ShaderManager shaderManager;
    SoundManager soundManager;
    FontManager fontManager;
    MusicManager musicManager;
    sf::Thread musicThread;

    Physica physicsManager;
    sf::Thread physicaThread;
    
    Constants::Status LoadFirstTimeGraphics();
    Constants::Status LoadAssets();
    void LogGraphicsSettings();
    void UpdatePerspective(unsigned int width, unsigned int height);
public:
    static Version Version;
    static Constants Constant;

    AsteroidaGraphica();
    Constants::Status Initialize();
    Constants::Status Run();
    void Deinitialize();
};

