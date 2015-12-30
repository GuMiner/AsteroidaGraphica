#pragma once
#include "ConfigManager.h"

class PhysicsConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();

public:
	static int PhysicsThreadDelay;

	static float BaseShipMass;
	static float SmallAsteroidMass;
	static float MediumAsteroidMass;
	static float LargeAsteroidMass;

	static float SolarMass;
	static float GravitationalConstant;

	static float AsteroidTimestep;
	static float AsteroidRotationSpeed;

	static float ShipThrustSpeed;
	static float ShipSideThrustSpeed;
	static float ShipVerticalThrustSpeed;
	static float TransDampenerThrustSpeed;

	static float ShipHorizRotSpeed;
	static float ShipVertRotSpeed;
	static float ShipBarrelRollSpeed;
	static float RotDampenerSpeed;

	PhysicsConfig(const char* configName);
};

