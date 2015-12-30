#pragma once
#include "ConfigManager.h"

class WorldGenerationConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();

public:
	static float SunSize;
	static float SunMaxPerPointDeformation;
	static float SunTriangleSize;

	static float SmallAsteroidSize;
	static float SmallAsteroidSizeMaxVariation;
	static float SmallAsteroidSizeMaxAxisDeformation;
	static float SmallAsteroidSizeMaxPerPointDeformation;
	static float SmallAsteroidTriangleSize;

	static float MediumAsteroidSize;
	static float MediumAsteroidSizeMaxVariation;
	static float MediumAsteroidSizeMaxAxisDeformation;
	static float MediumAsteroidSizeMaxPerPointDeformation;
	static float MediumAsteroidTriangleSize;

	static float LargeAsteroidSize;
	static float LargeAsteroidSizeMaxVariation;
	static float LargeAsteroidSizeMaxAxisDeformation;
	static float LargeAsteroidSizeMaxPerPointDeformation;
	static float LargeAsteroidTriangleSize;

	static float ForceFieldTriangleSize;

	static int SmallAsteroidTypes;
	static int MediumAsteroidTypes;
	static int LargeAsteroidTypes;

	static float AsteroidTorusMinDistance;
	static float AsteroidTorusRadius;
	static float AsteroidTorusHeight;

	static int AsteroidCount;

	WorldGenerationConfig(const char* configName);
};

