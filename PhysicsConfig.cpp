#include "Logger.h"
#include "PhysicsConfig.h"

int PhysicsConfig::PhysicsThreadDelay;

float PhysicsConfig::BaseShipMass;
float PhysicsConfig::SmallAsteroidMass;
float PhysicsConfig::MediumAsteroidMass;
float PhysicsConfig::LargeAsteroidMass;

float PhysicsConfig::SolarMass;
float PhysicsConfig::GravitationalConstant;

float PhysicsConfig::AsteroidTimestep;

float PhysicsConfig::AsteroidRotationSpeed;

float PhysicsConfig::ShipThrustSpeed;
float PhysicsConfig::ShipSideThrustSpeed;
float PhysicsConfig::ShipVerticalThrustSpeed;
float PhysicsConfig::TransDampenerThrustSpeed;

float PhysicsConfig::ShipHorizRotSpeed;
float PhysicsConfig::ShipVertRotSpeed;
float PhysicsConfig::ShipBarrelRollSpeed;
float PhysicsConfig::RotDampenerSpeed;

bool PhysicsConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
	LoadConfigurationValue(Int, PhysicsThreadDelay, "Error decoding the physics thread delay!");

	LoadConfigurationValue(Float, BaseShipMass, "Error decoding the basic ship mass!");
	LoadConfigurationValue(Float, SmallAsteroidMass, "Error decoding the small asteroid mass!");
	LoadConfigurationValue(Float, MediumAsteroidMass, "Error decoding the medium asteroid mass!");
	LoadConfigurationValue(Float, LargeAsteroidMass, "Error decoding the large asteroid mass!");
	LoadConfigurationValue(Float, SolarMass, "Error decoding the solar mass!");
	LoadConfigurationValue(Float, GravitationalConstant, "Error decoding the gravitational constant!");

	LoadConfigurationValue(Float, AsteroidTimestep, "Error decoding the asteroid timestep!");

	LoadConfigurationValue(Float, AsteroidRotationSpeed, "Error decoding the default asteroid rotation speed!");

	LoadConfigurationValue(Float, ShipThrustSpeed, "Error decoding the ship thrust force!");
	LoadConfigurationValue(Float, ShipSideThrustSpeed, "Error decoding the sideways ship thrust force!");
	LoadConfigurationValue(Float, ShipVerticalThrustSpeed, "Error decoding the vertical ship thrust force!");
	LoadConfigurationValue(Float, TransDampenerThrustSpeed, "Error decoding the translational dampener force!");

	LoadConfigurationValue(Float, ShipHorizRotSpeed, "Error decoding the horizontal gyroscope speed!");
	LoadConfigurationValue(Float, ShipVertRotSpeed, "Error decoding the vertical gyroscope speed!");
	LoadConfigurationValue(Float, ShipBarrelRollSpeed, "Error decoding the barrel roll gyroscope speed!");
	LoadConfigurationValue(Float, RotDampenerSpeed, "Error decoding the rotational dampener speed!");

	return true;
}

void PhysicsConfig::WriteConfigValues()
{
	WriteInt("PhysicsThreadDelay", PhysicsThreadDelay);

	WriteFloat("BaseShipMass", BaseShipMass);
	WriteFloat("SmallAsteroidMass", SmallAsteroidMass);
	WriteFloat("MediumAsteroidMass", MediumAsteroidMass);
	WriteFloat("LargeAsteroidMass", LargeAsteroidMass);
	WriteFloat("SolarMass", SolarMass);
	WriteFloat("GravtationalConstant", GravitationalConstant);

	WriteFloat("AsteroidTimestep", AsteroidTimestep);

	WriteFloat("AsteroidRotationSpeed", AsteroidRotationSpeed);

	WriteFloat("ShipThrustSpeed", ShipThrustSpeed);
	WriteFloat("ShipSideThrustSpeed", ShipSideThrustSpeed);
	WriteFloat("ShipVerticalThrustSpeed", ShipVerticalThrustSpeed);
	WriteFloat("TransDampenerThrustSpeed", TransDampenerThrustSpeed);

	WriteFloat("ShipHorizRotSpeed", ShipHorizRotSpeed);
	WriteFloat("ShipVertRotSpeed", ShipVertRotSpeed);
	WriteFloat("ShipBarrelRollSpeed", ShipBarrelRollSpeed);
	WriteFloat("RotDampenerSpeed", RotDampenerSpeed);
}

PhysicsConfig::PhysicsConfig(const char* configName)
	: ConfigManager(configName)
{
}