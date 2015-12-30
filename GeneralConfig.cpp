#include "Logger.h"
#include "GeneralConfig.h"

bool GeneralConfig::IsFullscreen;
int GeneralConfig::ScreenWidth;
int GeneralConfig::ScreenHeight;

float GeneralConfig::ViewDistance;

int GeneralConfig::TextImageSize;

int GeneralConfig::AsteroidRenderLimit;

bool GeneralConfig::SimpleForceFieldShader;
bool GeneralConfig::SimpleAsteroidLodShader;

bool GeneralConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
	LoadConfigurationValue(Bool, IsFullscreen, "Error decoding the fullscreen toggle!");
	LoadConfigurationValue(Int, ScreenWidth, "Error reading in the screen width!");
	LoadConfigurationValue(Int, ScreenHeight, "Error reading in the screen height!");
	LoadConfigurationValue(Float, ViewDistance, "Error reading in the asteroid view distance!");
	LoadConfigurationValue(Int, TextImageSize, "Error reading in the text image size!");

	LoadConfigurationValue(Int, AsteroidRenderLimit, "Error decoding the asteroid render limit!");

	LoadConfigurationValue(Bool, SimpleForceFieldShader, "Error decoding the simple force field shader toggle!");
	LoadConfigurationValue(Bool, SimpleAsteroidLodShader, "Error decoding the simple asteroid LOD shader toggle!");

	return true;
}

void GeneralConfig::WriteConfigValues()
{
	WriteBool("FullScreen", IsFullscreen);
	WriteInt("ScreenWidth", ScreenWidth);
	WriteInt("ScreenHeight", ScreenHeight);

	WriteFloat("ViewDistance", ViewDistance);

	WriteInt("TextImageSize", TextImageSize);

	WriteInt("AsteroidRenderLimit", AsteroidRenderLimit);
}

GeneralConfig::GeneralConfig(const char* configName)
	: ConfigManager(configName)
{
}