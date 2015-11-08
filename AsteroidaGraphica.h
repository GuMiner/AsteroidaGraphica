#pragma once

#include <memory>
#include <thread>
#include "Logger.hpp"
#include "Version.h"

class AsteroidaGraphica
{
public:
    // Held here as a common storage place.
    static std::unique_ptr<Logger> Log;
    static Version Version;

    AsteroidaGraphica();
    Version::Status Initialize();
    Version::Status Run();
    ~AsteroidaGraphica();
};

