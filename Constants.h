#pragma once
#include <random>

// Holds data constant across the application.
class Constants
{
    // Random number generation
    static std::mt19937 mersenneTwiser;
    static uint32_t seedValue;
    static std::uniform_real_distribution<float> uniformDistribution;

public:
    Constants()
    {
        FOV_Y = 50.0f;
        ASPECT = 1.77778f; // 16:9
        NEAR_PLANE = 0.10f;
        FAR_PLANE = 1000.0f;
        seedValue = 321641658; // Numpad-bashing.
        mersenneTwiser.seed(seedValue);
    }

    // Allowable return codes
    enum Status { OK = 0, 
        BAD_SETUP = 1, BAD_GLEW = 2, BAD_SHADERS = 3, 
        BAD_FONT = 4, BAD_IMAGES = 5, BAD_SOUND = 6, 
        BAD_MUSIC = 7, BAD_HUD = 8, BAD_CONFIG = 9,
        BAD_ASTEROIDA = 10 };

    // Graphics viewport settings
    static float FOV_Y;
    static float ASPECT;
    static float NEAR_PLANE;
    static float FAR_PLANE;

    // To support 144 Hz monitors.
    const static int MAX_FRAMERATE = 150;

    // Gets the next random floating-point value from 0 to 1, inclusive.
    static float Rand();

    // Gets the next random floating-point value from (-range/2 to range/2)
    static float Rand(float range);
};

