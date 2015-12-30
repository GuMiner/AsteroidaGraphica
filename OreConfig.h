#pragma once
#include "vmath.hpp"
#include "ConfigManager.h"

class OreConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();

public:
	// Colors
	static vmath::vec3 AsteroidGradientStartColor;
	static vmath::vec3 AsteroidGradientEndColor;

	static vmath::vec3 WaterOreColor;
	static vmath::vec3 FeOreColor;
	static vmath::vec3 SiOreColor;
	static vmath::vec3 CuOreColor;
	static vmath::vec3 UOreColor;
	static vmath::vec3 AuOreColor;
	static vmath::vec3 PtOreColor;
	static vmath::vec3 ImpOreColor;

	// Resources
	static float BaseWaterOre;
	static float MaxWaterOre;
	static float BaseFeOre;
	static float MaxFeOre;
	static float BaseSiOre;
	static float MaxSiOre;
	static float BaseCuOre;
	static float MaxCuOre;
	static float BaseUOre;
	static float MaxUOre;
	static float BaseAuOre;
	static float MaxAuOre;
	static float BasePtOre;
	static float MaxPtOre;
	static float BaseImpOre;
	static float MaxImpOre;

	static float OreEmptyRatio;
	static float WaterRatio;
	static float FeRatio;
	static float SiRatio;
	static float CuRatio;
	static float URatio;
	static float AuRatio;
	static float PtRatio;
	static float ImpRatio;

	OreConfig(const char* configName);
};

