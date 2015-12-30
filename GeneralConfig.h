#pragma once
#include "ConfigManager.h"

class GeneralConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();
public:
	// Graphics
	static bool IsFullscreen;
	static int ScreenWidth;
	static int ScreenHeight;

	static float ViewDistance;

	static int TextImageSize;

	static int AsteroidRenderLimit;

	static bool SimpleForceFieldShader;
	static bool SimpleAsteroidLodShader;

	GeneralConfig(const char* configName);
};

