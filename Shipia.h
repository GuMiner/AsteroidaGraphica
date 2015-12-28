#pragma once
#include "Elementa.h"
#include "SoundManager.h"
#include "vmath.hpp"

// Holds ship-specific data.
class Shipia
{
	Elementa* elementa;
	SoundManager *soundManager;

	// Make sure that the dampeners act as buttons instead of thrusters.
	bool rotDampToggled;
	bool tranDampToggled;

	void Thrust(bool forwards);
	void SideThrust(bool left);
	void VerticalThrust(bool up);
	void RotateHorizontal(bool left);
	void RotateVertical(bool up);
	void BarrelRoll(bool clockwise);

public:
	// Ship physical constants
	vmath::vec3 shipVelocity;
	vmath::vec3 shipForce;
	vmath::vec3 shipRotation;

	// Ship positioning
	vmath::vec3 shipPosition;
	vmath::quaternion shipOrientation;

	// Whether or not the dampeners are enabled.
	bool rotationalDampener;
	bool translationalDampener;

	Shipia();
	void Initialize(SoundManager *soundManager, Elementa *elementa);
	float GetShipMass() const;
	void Update(bool& didRotate, bool& didTranslate);
	~Shipia();
};

