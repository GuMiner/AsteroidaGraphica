#include "Logger.h"
#include "WorldGenerationConfig.h"

float WorldGenerationConfig::SunSize;
float WorldGenerationConfig::SunMaxPerPointDeformation;
float WorldGenerationConfig::SunTriangleSize;

float WorldGenerationConfig::SmallAsteroidSize;
float WorldGenerationConfig::SmallAsteroidSizeMaxVariation;
float WorldGenerationConfig::SmallAsteroidSizeMaxAxisDeformation;
float WorldGenerationConfig::SmallAsteroidSizeMaxPerPointDeformation;
float WorldGenerationConfig::SmallAsteroidTriangleSize;

float WorldGenerationConfig::MediumAsteroidSize;
float WorldGenerationConfig::MediumAsteroidSizeMaxVariation;
float WorldGenerationConfig::MediumAsteroidSizeMaxAxisDeformation;
float WorldGenerationConfig::MediumAsteroidSizeMaxPerPointDeformation;
float WorldGenerationConfig::MediumAsteroidTriangleSize;

float WorldGenerationConfig::LargeAsteroidSize;
float WorldGenerationConfig::LargeAsteroidSizeMaxVariation;
float WorldGenerationConfig::LargeAsteroidSizeMaxAxisDeformation;
float WorldGenerationConfig::LargeAsteroidSizeMaxPerPointDeformation;
float WorldGenerationConfig::LargeAsteroidTriangleSize;

float WorldGenerationConfig::ForceFieldTriangleSize;

int WorldGenerationConfig::SmallAsteroidTypes;
int WorldGenerationConfig::MediumAsteroidTypes;
int WorldGenerationConfig::LargeAsteroidTypes;

float WorldGenerationConfig::AsteroidTorusMinDistance;
float WorldGenerationConfig::AsteroidTorusRadius;
float WorldGenerationConfig::AsteroidTorusHeight;

int WorldGenerationConfig::AsteroidCount;

bool WorldGenerationConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
	LoadConfigurationValue(Float, SunSize, "Error reading in the sun size!");
	LoadConfigurationValue(Float, SunMaxPerPointDeformation, "Error reading in the sun max per point deformation!");
	LoadConfigurationValue(Float, SunTriangleSize, "Error reading in the sun triangle size!");
	LoadConfigurationValue(Float, SmallAsteroidSize, "Error reading in the small asteroid size!");
	LoadConfigurationValue(Float, SmallAsteroidSizeMaxVariation, "Error reading in the small asteroid size max variation!");
	LoadConfigurationValue(Float, SmallAsteroidSizeMaxAxisDeformation, "Error reading in the small asteroid size max axis deformation!");
	LoadConfigurationValue(Float, SmallAsteroidSizeMaxPerPointDeformation, "Error reading in the small asteroid size max per point deformation!");
	LoadConfigurationValue(Float, SmallAsteroidTriangleSize, "Error reading in the small asteroid triangle size!");
	LoadConfigurationValue(Float, MediumAsteroidSize, "Error reading in the medium asteroid size!");
	LoadConfigurationValue(Float, MediumAsteroidSizeMaxVariation, "Error reading in the medium asteroid size max variation!");
	LoadConfigurationValue(Float, MediumAsteroidSizeMaxAxisDeformation, "Error reading in the medium asteroid size max axis deformation!");
	LoadConfigurationValue(Float, MediumAsteroidSizeMaxPerPointDeformation, "Error reading in the medium asteroid size max per point deformation!");
	LoadConfigurationValue(Float, MediumAsteroidTriangleSize, "Error reading in the medium asteroid triangle size!");
	LoadConfigurationValue(Float, LargeAsteroidSize, "Error reading in the large asteroid size!");
	LoadConfigurationValue(Float, LargeAsteroidSizeMaxVariation, "Error reading in the large asteroid size max variation!");
	LoadConfigurationValue(Float, LargeAsteroidSizeMaxPerPointDeformation, "Error reading in the large asteroid size max axis deformation!");
	LoadConfigurationValue(Float, LargeAsteroidSizeMaxPerPointDeformation, "Error reading in the large asteroid size max per point deformation!");
	LoadConfigurationValue(Float, LargeAsteroidTriangleSize, "Error reading in the large asteroid triangle size!");

	LoadConfigurationValue(Float, ForceFieldTriangleSize, "Error reading in the force field triangle size!");

	LoadConfigurationValue(Int, SmallAsteroidTypes, "Error decoding the small asteroid type count!");
	LoadConfigurationValue(Int, MediumAsteroidTypes, "Error decoding the medium asteroid type count!");
	LoadConfigurationValue(Int, LargeAsteroidTypes, "Error decoding the large asteroid type count!");

	LoadConfigurationValue(Float, AsteroidTorusMinDistance, "Error decoding the torus min distance!");
	LoadConfigurationValue(Float, AsteroidTorusRadius, "Error decoding the torus radius!");
	LoadConfigurationValue(Float, AsteroidTorusHeight, "Error decoding the torus height!");

	LoadConfigurationValue(Int, AsteroidCount, "Error decoding the asteroid count!");

	return true;
}

void WorldGenerationConfig::WriteConfigValues()
{
	WriteFloat("SunSize", SunSize);
	WriteFloat("SunMaxPerPointDeformation", SunMaxPerPointDeformation);
	WriteFloat("SunTriangleSize", SunTriangleSize);

	WriteFloat("SmallAsteroidSize", SmallAsteroidSize);
	WriteFloat("SmallAsteroidSizeMaxVariation", SmallAsteroidSizeMaxVariation);
	WriteFloat("SmallAsteroidSizeMaxAxisDeformation", SmallAsteroidSizeMaxAxisDeformation);
	WriteFloat("SmallAsteroidSizeMaxPerPointDeformation", SmallAsteroidSizeMaxPerPointDeformation);
	WriteFloat("SmallAsteroidTriangleSize", SmallAsteroidTriangleSize);

	WriteFloat("MediumAsteroidSize", MediumAsteroidSize);
	WriteFloat("MediumAsteroidSizeMaxVariation", MediumAsteroidSizeMaxVariation);
	WriteFloat("MediumAsteroidSizeMaxAxisDeformation", MediumAsteroidSizeMaxAxisDeformation);
	WriteFloat("MediumAsteroidSizeMaxPerPointDeformation", MediumAsteroidSizeMaxPerPointDeformation);
	WriteFloat("MediumAsteroidTriangleSize", MediumAsteroidTriangleSize);

	WriteFloat("LargeAsteroidSize", LargeAsteroidSize);
	WriteFloat("LargeAsteroidSizeMaxVariation", LargeAsteroidSizeMaxVariation);
	WriteFloat("LargeAsteroidSizeMaxAxisDeformation", LargeAsteroidSizeMaxAxisDeformation);
	WriteFloat("LargeAsteroidSizeMaxPerPointDeformation", LargeAsteroidSizeMaxPerPointDeformation);
	WriteFloat("LargeAsteroidTriangleSize", LargeAsteroidTriangleSize);

	WriteFloat("ForceFieldTriangleSize", ForceFieldTriangleSize);

	WriteInt("SmallAsteroidTypes", SmallAsteroidTypes);
	WriteInt("MediumAsteroidTypes", MediumAsteroidTypes);
	WriteInt("LargeAsteroidTypes", LargeAsteroidTypes);

	WriteFloat("AsteroidTorusMinDistance", AsteroidTorusMinDistance);
	WriteFloat("AsteroidTorusRadius", AsteroidTorusRadius);
	WriteFloat("AsteroidTorusHeight", AsteroidTorusHeight);

	WriteInt("AsteroidCount", AsteroidCount);
}

WorldGenerationConfig::WorldGenerationConfig(const char* configName)
	: ConfigManager(configName)
{
}