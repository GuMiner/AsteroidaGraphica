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

	GeneralConfig(const char* configName);
};

