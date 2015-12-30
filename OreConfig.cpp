#include "Logger.h"
#include "OreConfig.h"

vmath::vec3 OreConfig::AsteroidGradientStartColor;
vmath::vec3 OreConfig::AsteroidGradientEndColor;

vmath::vec3 OreConfig::WaterOreColor;
vmath::vec3 OreConfig::FeOreColor;
vmath::vec3 OreConfig::SiOreColor;
vmath::vec3 OreConfig::CuOreColor;
vmath::vec3 OreConfig::UOreColor;
vmath::vec3 OreConfig::AuOreColor;
vmath::vec3 OreConfig::PtOreColor;
vmath::vec3 OreConfig::ImpOreColor;

float OreConfig::BaseWaterOre;
float OreConfig::MaxWaterOre;
float OreConfig::BaseFeOre;
float OreConfig::MaxFeOre;
float OreConfig::BaseSiOre;
float OreConfig::MaxSiOre;
float OreConfig::BaseCuOre;
float OreConfig::MaxCuOre;
float OreConfig::BaseUOre;
float OreConfig::MaxUOre;
float OreConfig::BaseAuOre;
float OreConfig::MaxAuOre;
float OreConfig::BasePtOre;
float OreConfig::MaxPtOre;
float OreConfig::BaseImpOre;
float OreConfig::MaxImpOre;

float OreConfig::OreEmptyRatio;
float OreConfig::WaterRatio;
float OreConfig::FeRatio;
float OreConfig::SiRatio;
float OreConfig::CuRatio;
float OreConfig::URatio;
float OreConfig::AuRatio;
float OreConfig::PtRatio;
float OreConfig::ImpRatio;

bool OreConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
	LoadConfigurationValue(Vector, AsteroidGradientStartColor, "Error decoding the asteroid gradient start color!");
	LoadConfigurationValue(Vector, AsteroidGradientEndColor, "Error decoding the asteroid gradient end color!");
	LoadConfigurationValue(Vector, WaterOreColor, "Error decoding the water ore color!");
	LoadConfigurationValue(Vector, FeOreColor, "Error decoding the iron ore color!");
	LoadConfigurationValue(Vector, SiOreColor, "Error decoding the silicon ore color!");
	LoadConfigurationValue(Vector, CuOreColor, "Error decoding the copper ore color!");
	LoadConfigurationValue(Vector, UOreColor, "Error decoding the uranium ore color!");
	LoadConfigurationValue(Vector, AuOreColor, "Error decoding the gold ore color!");
	LoadConfigurationValue(Vector, PtOreColor, "Error decoding the platinum ore color!");
	LoadConfigurationValue(Vector, ImpOreColor, "Error decoding the impurity ore color!");

	LoadConfigurationValue(Float, BaseWaterOre, "Error decoding the base water ore amount!");
	LoadConfigurationValue(Float, MaxWaterOre, "Error decoding the max water ore amount!");
	LoadConfigurationValue(Float, BaseFeOre, "Error decoding the base iron ore amount!");
	LoadConfigurationValue(Float, MaxFeOre, "Error decoding the max iron ore amount!");
	LoadConfigurationValue(Float, BaseSiOre, "Error decoding the base silicon ore amount!");
	LoadConfigurationValue(Float, MaxSiOre, "Error decoding the max silicon ore amount!");
	LoadConfigurationValue(Float, BaseCuOre, "Error decoding the base copper ore amount!");
	LoadConfigurationValue(Float, MaxCuOre, "Error decoding the max copper ore amount!");
	LoadConfigurationValue(Float, BaseUOre, "Error decoding the base uranium ore amount!");
	LoadConfigurationValue(Float, MaxUOre, "Error decoding the max uranium ore amount!");
	LoadConfigurationValue(Float, BaseAuOre, "Error decoding the base gold ore amount!");
	LoadConfigurationValue(Float, MaxAuOre, "Error decoding the max gold ore amount!");
	LoadConfigurationValue(Float, BasePtOre, "Error decoding the base platinum ore amount!");
	LoadConfigurationValue(Float, MaxPtOre, "Error decoding the max platinum ore amount!");
	LoadConfigurationValue(Float, BaseImpOre, "Error decoding the base impurities ore amount!");
	LoadConfigurationValue(Float, MaxImpOre, "Error decoding the max impurities ore amount!");

	LoadConfigurationValue(Float, OreEmptyRatio, "Error decoding the ore/no ore ratio!");
	LoadConfigurationValue(Float, WaterRatio, "Error decoding the water ratio!");
	LoadConfigurationValue(Float, FeRatio, "Error decoding the iron ratio!");
	LoadConfigurationValue(Float, SiRatio, "Error decoding the silicon ratio!");
	LoadConfigurationValue(Float, CuRatio, "Error decoding the copper ratio!");
	LoadConfigurationValue(Float, URatio, "Error decoding the uranium ratio!");
	LoadConfigurationValue(Float, AuRatio, "Error decoding the gold ratio!");
	LoadConfigurationValue(Float, PtRatio, "Error decoding the platinum ratio!");
	LoadConfigurationValue(Float, ImpRatio, "Error decoding the impurities ratio!");

	return true;
}

void OreConfig::WriteConfigValues()
{
	WriteVector("AsteroidGradientStartColor", AsteroidGradientStartColor);
	WriteVector("AsteroidGradientEndColor", AsteroidGradientEndColor);

	WriteVector("WaterOreColor", WaterOreColor);
	WriteVector("FeOreColor", FeOreColor);
	WriteVector("SiOreColor", SiOreColor);
	WriteVector("CuOreColor", CuOreColor);
	WriteVector("UOreColor", UOreColor);
	WriteVector("AuOreColor", AuOreColor);
	WriteVector("PtOreColor", PtOreColor);
	WriteVector("ImpOreColor", ImpOreColor);

	WriteFloat("BaseWaterOre", BaseWaterOre);
	WriteFloat("MaxWaterOre", MaxWaterOre);
	WriteFloat("BaseFeOre", BaseFeOre);
	WriteFloat("MaxFeOre", MaxFeOre);
	WriteFloat("BaseSiOre", BaseSiOre);
	WriteFloat("MaxSiOre", MaxSiOre);
	WriteFloat("BaseCuOre", BaseCuOre);
	WriteFloat("MaxCuOre", MaxCuOre);
	WriteFloat("BaseUOre", BaseUOre);
	WriteFloat("MaxUOre", MaxUOre);
	WriteFloat("BaseAuOre", BaseAuOre);
	WriteFloat("MaxAuOre", MaxAuOre);
	WriteFloat("BasePtOre", BasePtOre);
	WriteFloat("MaxPtOre", MaxPtOre);
	WriteFloat("BaseImpOre", BaseImpOre);
	WriteFloat("MaxImpOre", MaxImpOre);

	WriteFloat("OreEmptyRatio", OreEmptyRatio);
	WriteFloat("WaterRatio", WaterRatio);
	WriteFloat("FeRatio", FeRatio);
	WriteFloat("SiRatio", SiRatio);
	WriteFloat("CuRatio", CuRatio);
	WriteFloat("URatio", URatio);
	WriteFloat("AuRatio", AuRatio);
	WriteFloat("PtRatio", PtRatio);
	WriteFloat("ImpRatio", ImpRatio);
}

OreConfig::OreConfig(const char* configName)
	: ConfigManager(configName)
{
}
