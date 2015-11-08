#pragma once

#include <string>

// Holds version information that is constant across the application.
class Version
{
public:
    explicit Version()
    {
        NAME = std::string("AsteroidaGraphica");
    }

    static std::string NAME;
    const static int MAJOR_VERSION = 1;
    const static int MINOR_VERSION = 10;

    enum Status { OK = 0, BAD_SETUP = 1};
};