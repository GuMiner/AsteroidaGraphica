#pragma once

#include <memory>
#include <thread>
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

    GLuint textureShaderProgram;
    GLint mvTexLocation, projTexLocation;

    GLuint flatTextureShaderProgram;
    GLint projFlatTexLocation;
    
    // TEST OBJECTS
    vmath::mat4 lookAtMatrix;
    GLuint vao;
    GLuint pointBuffer;
    GLsizei vertexCount;

    ImageManager imageManager;
    ShaderManager shaderManager;
    SoundManager soundManager;
    FontManager fontManager;

    Physica physicsManager;
    sf::Thread physicaThread;

    MusicManager musicManager;
    sf::Thread musicThread;

    Constants::Status LoadFirstTimeGraphics();
    Constants::Status LoadAssets(GLuint& compassTexture);
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

