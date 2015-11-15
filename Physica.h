#pragma once
#include "vmath.hpp"

// Manages object physics.
class Physica
{
    // Ship physical constants
    vmath::vec3 shipVelocity;
    vmath::vec3 shipForce;
    float shipMass;

    bool isAlive;
    bool isPaused;

    void Thrust(bool forwards);
    void SideThrust(bool left);
    void VerticalThrust(bool up);
    void RotateHorizontal(bool left);
    void RotateVertical(bool up);
public:
    // Ship positioning
    vmath::vec3 shipPosition;
    vmath::quaternion shipOrientation;

    Physica();
    void Run();

    void Pause()
    {
        isPaused = true;
    }

    void Resume()
    {
        isPaused = false;
    }

    void Stop();
    ~Physica();
};

