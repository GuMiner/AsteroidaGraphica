#pragma once
#include <memory>
#include <vector>
#include "Asteroida.h"
#include "Shipia.h"
#include "Stellaria.h"
#include "vmath.hpp"

// The position-velocity structure used to simplify physics calculations.
struct PosVel
{
	vmath::vec3 pos;
	vmath::vec3 vel;
};

// Manages object physics.
class Physica
{
    Asteroida *asteroida;
	Shipia *shipia;
	Stellaria *stellaria;

    bool didTranslate;
    bool didRotate;

    bool isAlive;
    bool isPaused;
    bool initialized;

    void HandleShipMotion();
	void HandleAsteroidMotion();

	// Physics calculations. 
	// FW: Ideally, I'd like to combine these so we can pass in a function pointer and only duplicate Sun/ShipAcceleration.
	vmath::vec3 SunAcceleration(const vmath::vec3& pos) const;
	PosVel RK4SunStepEvaluate(const PosVel& initialState, float dt, const PosVel& derivative) const;
	void SunIntegration(vmath::vec3& pos, vmath::vec3& vel, float dt) const;

	vmath::vec3 ShipAcceleration(const vmath::vec3& pos) const;
	PosVel RK4ShipStepEvaluate(const PosVel& initialState, float dt, const PosVel& derivative) const;
	void ShipIntegration(vmath::vec3& pos, vmath::vec3& vel, float dt) const;
public:

    Physica();
    void Initialize(Asteroida* asteroida, Stellaria* stellaria, Shipia* shipia);
    void Run();

    void Pause()
    {
        isPaused = true;
    }
    void Resume()
    {
        isPaused = false;
    }
    void Stop()
    {
        isAlive = false;
    }

    ~Physica();
};

