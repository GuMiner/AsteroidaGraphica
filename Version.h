#pragma once

#include <string>

// Holds version information that is constant across the application.
class Version
{
public:
    explicit Version()
    {
        NAME = std::string("AsteroidaGraphica");
        FOV_Y = 50.0f;
        ASPECT = 1.77778f; // 16:9
        NEAR_PLANE = 0.10f;
        FAR_PLANE = 1000.0f;
    }

    // Name and version details
    static std::string NAME;
    const static int MAJOR_VERSION = 1;
    const static int MINOR_VERSION = 461;

    // Allowable return codes
    enum Status { OK = 0, BAD_SETUP = 1, BAD_GLEW = 2, BAD_SHADERS = 3, BAD_FONT = 4, BAD_IMAGES = 5, BAD_SOUND = 6, BAD_MUSIC = 7 };
    
    // Graphics viewport settings
    static float FOV_Y;
    static float ASPECT;
    static float NEAR_PLANE;
    static float FAR_PLANE;

    // To support 144 Hz monitors.
    const static int MAX_FRAMERATE = 150;
};