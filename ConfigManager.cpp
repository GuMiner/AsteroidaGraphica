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

vmath::vec3 ConfigManager::AsteroidGradientStartColor;
vmath::vec3 ConfigManager::AsteroidGradientEndColor;

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

	currentLine = configFileLines[++lineCounter];
    if (!LoadBool(IsFullscreen))
    {
        Logger::Log("Error decoding the fullscreen toggle!");
        return false;
    }

	currentLine = configFileLines[++lineCounter];
    if (!LoadInt(ScreenWidth))
    {
        Logger::Log("Error reading in the screen width!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadInt(ScreenHeight))
    {
        Logger::Log("Error reading in the screen height!");
        return false;
    }

	currentLine = configFileLines[++lineCounter];
	if (!LoadInt(TextImageSize))
	{
		Logger::Log("Error reading in the text image size!");
		return false;
	}

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ThrustForwardsKey))
    {
        Logger::Log("Error reading in the thrust forwards key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ThrustReverseKey))
    {
        Logger::Log("Error reading in the thrust reverse key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ThrustLeftKey))
    {
        Logger::Log("Error reading in the thrust left key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ThrustRightKey))
    {
        Logger::Log("Error reading in the thrust right key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ThrustUpKey))
    {
        Logger::Log("Error reading in the thrust up key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ThrustDownKey))
    {
        Logger::Log("Error reading in the thrust down key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(RotateLeftKey))
    {
        Logger::Log("Error reading in the rotate left key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(RotateRightKey))
    {
        Logger::Log("Error reading in the rotate right key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(RotateUpKey))
    {
        Logger::Log("Error reading in the rotate up key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(RotateDownKey))
    {
        Logger::Log("Error reading in the rotate down key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(RotateCWKey))
    {
        Logger::Log("Error reading in the rotate CW key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(RotateCCWKey))
    {
        Logger::Log("Error reading in the rotate CCW key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ToggleRotationDampeningKey))
    {
        Logger::Log("Error reading in the rotation dampening toggle key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(ToggleTranslationDampeningKey))
    {
        Logger::Log("Error reading in the translational dampening toggle key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadKey(PauseKey))
    {
        Logger::Log("Error reading in the pause key!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SunSize))
    {
        Logger::Log("Error reading in the sun size!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SunMaxPerPointDeformation))
    {
        Logger::Log("Error reading in the sun max per point deformation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SunTriangleSize))
    {
        Logger::Log("Error reading in the sun triangle size!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SmallAsteroidSize))
    {
        Logger::Log("Error reading in the small asteroid size!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SmallAsteroidSizeMaxVariation))
    {
        Logger::Log("Error reading in the small asteroid size max variation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SmallAsteroidSizeMaxAxisDeformation))
    {
        Logger::Log("Error reading in the small asteroid size max axis deformation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SmallAsteroidSizeMaxPerPointDeformation))
    {
        Logger::Log("Error reading in the small asteroid size max per point deformation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(SmallAsteroidTriangleSize))
    {
        Logger::Log("Error reading in the small asteroid triangle size!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(MediumAsteroidSize))
    {
        Logger::Log("Error reading in the medium asteroid size!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(MediumAsteroidSizeMaxVariation))
    {
        Logger::Log("Error reading in the medium asteroid size max variation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(MediumAsteroidSizeMaxAxisDeformation))
    {
        Logger::Log("Error reading in the medium asteroid size max axis deformation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(MediumAsteroidSizeMaxPerPointDeformation))
    {
        Logger::Log("Error reading in the medium asteroid size max per point deformation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(MediumAsteroidTriangleSize))
    {
        Logger::Log("Error reading in the medium asteroid triangle size!");
        return false;
    }
    
    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(LargeAsteroidSize))
    {
        Logger::Log("Error reading in the large asteroid size!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(LargeAsteroidSizeMaxVariation))
    {
        Logger::Log("Error reading in the large asteroid size max variation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(LargeAsteroidSizeMaxPerPointDeformation))
    {
        Logger::Log("Error reading in the large asteroid size max axis deformation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(LargeAsteroidSizeMaxPerPointDeformation))
    {
        Logger::Log("Error reading in the large asteroid size max per point deformation!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadFloat(LargeAsteroidTriangleSize))
    {
        Logger::Log("Error reading in the large asteroid triangle size!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadInt(SmallAsteroidTypes))
    {
        Logger::Log("Error decoding the small asteroid type count!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadInt(MediumAsteroidTypes))
    {
        Logger::Log("Error decoding the medium asteroid type count!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadInt(LargeAsteroidTypes))
    {
        Logger::Log("Error decoding the large asteroid type count!");
        return false;
    }

    currentLine = configFileLines[++lineCounter];
    if (!LoadInt(AsteroidCount))
    {
        Logger::Log("Error decoding the asteroid count!");
        return false;
    }

	currentLine = configFileLines[++lineCounter];
	if (!LoadVector(AsteroidGradientStartColor))
	{
		Logger::Log("Error decoding the asteroid gradient start color!");
		return false;
	}

	currentLine = configFileLines[++lineCounter];
	if (!LoadVector(AsteroidGradientEndColor))
	{
		Logger::Log("Error decoding the asteroid gradient end color!");
		return false;
	}

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

	WriteVector("AsteroidGradientStartColor", AsteroidGradientStartColor);
	WriteVector("AsteroidGradientEndColor", AsteroidGradientEndColor);
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
