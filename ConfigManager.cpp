#include <sstream>
#include "Logger.h"
#include "StringUtils.h"
#include "ConfigManager.h"

int ConfigManager::ConfigVersion;

bool ConfigManager::IsFullscreen;
int ConfigManager::ScreenWidth;
int ConfigManager::ScreenHeight;

int ConfigManager::TextImageSize;

sf::Keyboard::Key ConfigManager::ThrustForwardsKey;
sf::Keyboard::Key ConfigManager::ThrustReverseKey;
sf::Keyboard::Key ConfigManager::ThrustLeftKey;
sf::Keyboard::Key ConfigManager::ThrustRightKey;
sf::Keyboard::Key ConfigManager::ThrustUpKey;
sf::Keyboard::Key ConfigManager::ThrustDownKey;

sf::Keyboard::Key ConfigManager::RotateLeftKey;
sf::Keyboard::Key ConfigManager::RotateRightKey;
sf::Keyboard::Key ConfigManager::RotateUpKey;
sf::Keyboard::Key ConfigManager::RotateDownKey;
sf::Keyboard::Key ConfigManager::RotateCWKey;
sf::Keyboard::Key ConfigManager::RotateCCWKey;

sf::Keyboard::Key ConfigManager::ToggleRotationDampeningKey;
sf::Keyboard::Key ConfigManager::ToggleTranslationDampeningKey;
sf::Keyboard::Key ConfigManager::PauseKey;

float ConfigManager::SunSize;
float ConfigManager::SunMaxPerPointDeformation;
float ConfigManager::SunTriangleSize;

float ConfigManager::SmallAsteroidSize;
float ConfigManager::SmallAsteroidSizeMaxVariation;
float ConfigManager::SmallAsteroidSizeMaxAxisDeformation;
float ConfigManager::SmallAsteroidSizeMaxPerPointDeformation;
float ConfigManager::SmallAsteroidTriangleSize;

float ConfigManager::MediumAsteroidSize;
float ConfigManager::MediumAsteroidSizeMaxVariation;
float ConfigManager::MediumAsteroidSizeMaxAxisDeformation;
float ConfigManager::MediumAsteroidSizeMaxPerPointDeformation;
float ConfigManager::MediumAsteroidTriangleSize;

float ConfigManager::LargeAsteroidSize;
float ConfigManager::LargeAsteroidSizeMaxVariation;
float ConfigManager::LargeAsteroidSizeMaxAxisDeformation;
float ConfigManager::LargeAsteroidSizeMaxPerPointDeformation;
float ConfigManager::LargeAsteroidTriangleSize;

int ConfigManager::SmallAsteroidTypes;
int ConfigManager::MediumAsteroidTypes;
int ConfigManager::LargeAsteroidTypes;

int ConfigManager::AsteroidCount;

int ConfigManager::PhysicsThreadDelay;

float ConfigManager::BaseShipMass;
float ConfigManager::SmallAsteroidMass;
float ConfigManager::MediumAsteroidMass;
float ConfigManager::LargeAsteroidMass;

float ConfigManager::SolarMass;
float ConfigManager::GravitationalConstant;

float ConfigManager::AsteroidTimestep;

float ConfigManager::AsteroidRotationSpeed;

vmath::vec3 ConfigManager::AsteroidGradientStartColor;
vmath::vec3 ConfigManager::AsteroidGradientEndColor;

vmath::vec3 ConfigManager::WaterOreColor;
vmath::vec3 ConfigManager::FeOreColor;
vmath::vec3 ConfigManager::SiOreColor;
vmath::vec3 ConfigManager::CuOreColor;
vmath::vec3 ConfigManager::UOreColor;
vmath::vec3 ConfigManager::AuOreColor;
vmath::vec3 ConfigManager::PtOreColor;
vmath::vec3 ConfigManager::ImpOreColor;

float ConfigManager::BaseWaterOre;
float ConfigManager::MaxWaterOre;
float ConfigManager::BaseFeOre;
float ConfigManager::MaxFeOre;
float ConfigManager::BaseSiOre;
float ConfigManager::MaxSiOre;
float ConfigManager::BaseCuOre;
float ConfigManager::MaxCuOre;
float ConfigManager::BaseUOre;
float ConfigManager::MaxUOre;
float ConfigManager::BaseAuOre;
float ConfigManager::MaxAuOre;
float ConfigManager::BasePtOre;
float ConfigManager::MaxPtOre;
float ConfigManager::BaseImpOre;
float ConfigManager::MaxImpOre;

ConfigManager::ConfigManager()
{
    CommentString = "#";
    configFileName = "config/config.txt";
}

// Loads in a boolean configuration value.
bool ConfigManager::LoadBool(bool& boolean)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(currentLine, tempInput) || !StringUtils::ParseBoolFromString(tempInput, boolean));
}

// Loads in an integer configuration value.
bool ConfigManager::LoadInt(int& integer)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(currentLine, tempInput) || !StringUtils::ParseIntFromString(tempInput, integer));
}

// Loads in a floating-point configuration value.
bool ConfigManager::LoadFloat(float& floatingPoint)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(currentLine, tempInput) || !StringUtils::ParseFloatFromString(tempInput, floatingPoint));
}

// Loads in an SFML keyboard key.
bool ConfigManager::LoadKey(sf::Keyboard::Key& key)
{
    int keyInt;
    if (!LoadInt(keyInt))
    {
        return false;
    }

    key = (sf::Keyboard::Key)keyInt;
    return true;
}

// Loads in a 3-valued floating point vector.
bool ConfigManager::LoadVector(vmath::vec3& vector)
{
	std::vector<std::string> stringParts;
	StringUtils::Split(currentLine, StringUtils::Space, true, stringParts);

	if (stringParts.size() != 4)
	{
		return false;
	}

	if (!StringUtils::ParseFloatFromString(stringParts[1], vector[0]) ||
		!StringUtils::ParseFloatFromString(stringParts[2], vector[1]) ||
		!StringUtils::ParseFloatFromString(stringParts[3], vector[2]))
	{
		return false;
	}

	return true;
}

void ConfigManager::WriteBool(const char* itemName, bool& boolean)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << std::boolalpha << boolean;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteInt(const char* itemName, int& integer)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << integer;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteFloat(const char* itemName, float& floatingPoint)
{
    double tempValue = floatingPoint;
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << tempValue;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteKey(const char* itemName, sf::Keyboard::Key& key)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << (int)key;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteVector(const char* itemName, vmath::vec3& vector)
{
	double x = vector[0];
	double y = vector[1];
	double z = vector[2];
	std::stringstream tempOutput;
	tempOutput << itemName << StringUtils::Space << x << StringUtils::Space << y << StringUtils::Space << z;
	outputLines.push_back(tempOutput.str());

}

// Function macro to wrap calls to Load* and spit out an error message on failure.
#define LoadConfigurationValue(Type, Variable, ErrorMessage) \
    currentLine = configFileLines[++lineCounter];			 \
    if (!Load##Type(##Variable))		  					 \
	{														 \
		Logger::Log(##ErrorMessage);                         \
		return false;										 \
	}														 \

// Loads in all the configuration values.
bool ConfigManager::LoadConfigurationValues(std::vector<std::string>& configFileLines)
{
    int lineCounter = 0;
	
	currentLine = configFileLines[lineCounter];
    if (!LoadInt(ConfigVersion))
    {
        Logger::Log("Error decoding the configuration file version!");
        return false;
    }

	// These calls are all the same as above, just using the function-esc macro.
	LoadConfigurationValue(Bool, IsFullscreen, "Error decoding the fullscreen toggle!");
	LoadConfigurationValue(Int, ScreenWidth, "Error reading in the screen width!");
    LoadConfigurationValue(Int, ScreenHeight, "Error reading in the screen height!");
	LoadConfigurationValue(Int, TextImageSize, "Error reading in the text image size!");

	LoadConfigurationValue(Key, ThrustForwardsKey, "Error reading in the thrust forwards key!");
    LoadConfigurationValue(Key, ThrustReverseKey, "Error reading in the thrust reverse key!");
	LoadConfigurationValue(Key, ThrustLeftKey, "Error reading in the thrust left key!");
    LoadConfigurationValue(Key, ThrustRightKey, "Error reading in the thrust right key!");
	LoadConfigurationValue(Key, ThrustUpKey, "Error reading in the thrust up key!");
	LoadConfigurationValue(Key, ThrustDownKey, "Error reading in the thrust down key!");
	LoadConfigurationValue(Key, RotateLeftKey, "Error reading in the rotate left key!");
	LoadConfigurationValue(Key, RotateRightKey, "Error reading in the rotate right key!");
    LoadConfigurationValue(Key, RotateUpKey, "Error reading in the rotate up key!");
	LoadConfigurationValue(Key, RotateDownKey, "Error reading in the rotate down key!");
	LoadConfigurationValue(Key, RotateCWKey, "Error reading in the rotate CW key!");
	LoadConfigurationValue(Key, RotateCCWKey ,"Error reading in the rotate CCW key!");
	LoadConfigurationValue(Key, ToggleRotationDampeningKey, "Error reading in the rotation dampening toggle key!");
	LoadConfigurationValue(Key, ToggleTranslationDampeningKey, "Error reading in the translational dampening toggle key!");
	LoadConfigurationValue(Key, PauseKey, "Error reading in the pause key!");

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


	LoadConfigurationValue(Int, SmallAsteroidTypes, "Error decoding the small asteroid type count!");
	LoadConfigurationValue(Int, MediumAsteroidTypes, "Error decoding the medium asteroid type count!");
	LoadConfigurationValue(Int, LargeAsteroidTypes, "Error decoding the large asteroid type count!");
	LoadConfigurationValue(Int, AsteroidCount, "Error decoding the asteroid count!");

	LoadConfigurationValue(Int, PhysicsThreadDelay, "Error decoding the physics thread delay!");

	LoadConfigurationValue(Float, BaseShipMass, "Error decoding the basic ship mass!");
	LoadConfigurationValue(Float, SmallAsteroidMass, "Error decoding the small asteroid mass!");
	LoadConfigurationValue(Float, MediumAsteroidMass, "Error decoding the medium asteroid mass!");
	LoadConfigurationValue(Float, LargeAsteroidMass, "Error decoding the large asteroid mass!");
	LoadConfigurationValue(Float, SolarMass, "Error decoding the solar mass!");
	LoadConfigurationValue(Float, GravitationalConstant, "Error decoding the gravitational constant!");

	LoadConfigurationValue(Float, AsteroidTimestep, "Error decoding the asteroid timestep!");
	
	LoadConfigurationValue(Float, AsteroidRotationSpeed, "Error decoding the default asteroid rotation speed!");

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
	LoadConfigurationValue(Float, MaxImpOre, "Error decoding the max imputiries ore amount!");

    return true;
}

void ConfigManager::WriteConfigurationValues()
{
    WriteInt("ConfigVersion", ConfigVersion);
    
    WriteBool("FullScreen", IsFullscreen);
    WriteInt("ScreenWidth", ScreenWidth);
    WriteInt("ScreenHeight", ScreenHeight);

	WriteInt("TextImageSize", TextImageSize);

    WriteKey("ThrustForwards", ThrustForwardsKey);
    WriteKey("ThrustReverse", ThrustReverseKey);
    WriteKey("ThrustLeft", ThrustLeftKey);
    WriteKey("ThrustRight", ThrustRightKey);
    WriteKey("ThrustUp", ThrustUpKey);
    WriteKey("ThrustDown", ThrustDownKey);
    
    WriteKey("RotateLeft", RotateLeftKey);
    WriteKey("RotateRight", RotateRightKey);
    WriteKey("RotateUp", RotateUpKey);
    WriteKey("RotateDown", RotateDownKey);
    WriteKey("RotateCW", RotateCWKey);
    WriteKey("RotateCCW", RotateCCWKey);
    
    WriteKey("ToggleRotationDampening", ToggleRotationDampeningKey);
    WriteKey("ToggleTranslationDampening", ToggleTranslationDampeningKey);
    WriteKey("Pause", PauseKey);

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

    WriteInt("SmallAsteroidTypes", SmallAsteroidTypes);
    WriteInt("MediumAsteroidTypes", MediumAsteroidTypes);
    WriteInt("LargeAsteroidTypes", LargeAsteroidTypes);

    WriteInt("AsteroidCount", AsteroidCount);

	WriteInt("PhysicsThreadDelay", PhysicsThreadDelay);

	WriteFloat("BaseShipMass", BaseShipMass);
	WriteFloat("SmallAsteroidMass", SmallAsteroidMass);
	WriteFloat("MediumAsteroidMass", MediumAsteroidMass);
	WriteFloat("LargeAsteroidMass", LargeAsteroidMass);
	WriteFloat("SolarMass", SolarMass);
	WriteFloat("GravtationalConstant", GravitationalConstant);

	WriteFloat("AsteroidTimestep", AsteroidTimestep);

	WriteFloat("AsteroidRotationSpeed", AsteroidRotationSpeed);

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
	WriteFloat("BaseImpOre",BaseImpOre);
	WriteFloat("MaxImpOre", MaxImpOre);
}

// Reads in the configuration and sets up the variables listed
bool ConfigManager::ReadConfiguration()
{
    // Load, split, and remove comment lines from the configuration file.
    std::string entireFile;
    std::vector<std::string> lines;
    if (!StringUtils::LoadStringFromFile(configFileName, entireFile))
    {
        Logger::Log("Unable to load the config file!");
        return false;
    }

    int currentLine = 0;
    StringUtils::Split(entireFile, StringUtils::Newline, false, lines);
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        if (StringUtils::StartsWith(lines[i], CommentString))
        {
            commentLines[currentLine] = std::string(lines[i]);

            lines.erase(lines.begin() + i);
            i--;
        }
        else if (StringUtils::IsWhitespaceOrEmpty(lines[i]))
        {
            commentLines[currentLine] = std::string(lines[i]);

            lines.erase(lines.begin() + i);
            i--;
        }
        
        ++currentLine;
    }

    // Parse out the configuration values from the file.
    if (!LoadConfigurationValues(lines))
    {
        Logger::Log("Invalid values were found in the config file!");
        return false;
    }

    return true;
}

// Writes out any changes to the static variables to the configuration.
bool ConfigManager::WriteConfiguration()
{
    // Write out our config file, ensuring we re-insert comment lines as appropriate.
	outputLines.clear();
    WriteConfigurationValues();

    for (unsigned int i = 0; i < outputLines.size(); i++)
    {
        if (commentLines.count(i) != 0)
        {
            // There's a comment line at this position, so add it in.
			outputLines.insert(outputLines.begin() + i, commentLines[i]);
        }
    }

    std::stringstream resultingFile;
    for (unsigned int i = 0; i < outputLines.size(); i++)
    {
        resultingFile << outputLines[i] << StringUtils::Newline;
    }

    if (!StringUtils::WriteStringToFile(configFileName, resultingFile.str()))
    {
        Logger::Log("Couldn't write out the updated config file!");
        return false;
    }

    return true;
}
