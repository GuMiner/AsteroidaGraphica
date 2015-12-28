#pragma once

#include <memory>
#include <thread>
#include "Asteroida.h"
#include "ConfigManager.h"
#include "Elementa.h"
#include "FontManager.h"
#include "ImageManager.h"
#include "Logger.h"
#include "MusicManager.h"
#include "Physica.h"
#include "ShaderManager.h"
#include "Shipia.h"
#include "ShipHud.h"
#include "SoundManager.h"
#include "Stellaria.h"
#include "Version.h"
#include "vmath.hpp"

// Handles startup and the base graphics rendering loop.
class AsteroidaGraphica
{
    // Game Data
    Asteroida asteroida;
	Shipia shipia;
	Elementa elementa;
	Stellaria stellaria;
    ShipHud shipHud;
    vmath::mat4 perspectiveMatrix;

    Physica physicsManager;
    sf::Thread physicaThread;

    // Managers
    ConfigManager configManager;
    ImageManager imageManager;
    ShaderManager shaderManager;
    SoundManager soundManager;
    FontManager fontManager;
    MusicManager musicManager;
    sf::Thread musicThread;
        
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

