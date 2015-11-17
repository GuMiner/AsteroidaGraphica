#pragma once
#include <memory>
#include "SoundManager.h"
#include "vmath.hpp"

// Manages object physics.
class Physica
{
    SoundManager *soundManager;

    // Ship physical constants
    vmath::vec3 shipVelocity;
    vmath::vec3 shipForce;
    vmath::vec3 shipRotation;
    float shipMass;
    bool rotationalDampener;
    bool translationalDampener;

    // Make sure that the dampeners act as buttons instead of thrusters.
    bool rotDampToggled;
    bool tranDampToggled;

    bool didTranslate;
    bool didRotate;

    bool isAlive;
    bool isPaused;

    float shipThrustSpeed;
    float shipSideThrustSpeed;
    float shipVerticalThrustSpeed;
    float shipHorizRotSpeed;
    float shipVertRotSpeed;
    float shipBarrelRollSpeed;

    float rotDampenerSpeed;
    float transDampenerThrustSpeed;

    void Thrust(bool forwards);
    void SideThrust(bool left);
    void VerticalThrust(bool up);
    void RotateHorizontal(bool left);
    void RotateVertical(bool up);
    void BarrelRoll(bool clockwise);

    void HandleShipControls();
    void HandleShipMotion();
public:
    // Ship positioning
    vmath::vec3 shipPosition;
    vmath::quaternion shipOrientation;

    Physica();
    void Initialize(SoundManager *soundManager);
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

