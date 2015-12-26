#pragma once

// Holds version information that is constant across the application.
// This import should be *sparingly* used, because it updates every build.
class Version
{
public:
    Version()
    {
        NAME = "AsteroidaGraphica";
    }

    // Name and version details
    static const char* NAME;
    const static int MAJOR_VERSION = 1;
    const static int MINOR_VERSION = 946;
};