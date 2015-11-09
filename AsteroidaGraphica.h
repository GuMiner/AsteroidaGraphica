#pragma once

#include <memory>
#include <thread>
#include "Logger.hpp"
#include "Physica.h"
#include "Version.h"

// Handles startup and the base graphics rendering loop.
class AsteroidaGraphica
{
    Physica physicsManager;
    sf::Thread physicaThread;
public:
    // Held here as a common storage place.
    static std::unique_ptr<Logger> Log;
    static Version Version;

    AsteroidaGraphica();
    Version::Status Initialize();
    Version::Status Run();
    void Deinitialize();
};

