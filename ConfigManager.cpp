#include <sstream>
#include "Logger.h"
#include "StringUtils.h"
#include "ConfigManager.h"

int ConfigManager::ConfigVersion;

bool ConfigManager::IsFullscreen;
int ConfigManager::ScreenWidth;
int ConfigManager::ScreenHeight;

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

ConfigManager::ConfigManager()
{
    CommentString = "#";
    configFileName = "config/config.txt";
}

// Loads in a boolean configuration value.
bool ConfigManager::LoadBool(std::string& line, bool& boolean)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseBoolFromString(tempInput, boolean));
}

// Loads in an integer configuration value.
bool ConfigManager::LoadInt(std::string& line, int& integer)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseIntFromString(tempInput, integer));
}

// Loads in an SFML keyboard key.
bool ConfigManager::LoadKey(std::string& line, sf::Keyboard::Key& key)
{
    int keyInt;
    if (!LoadInt(line, keyInt))
    {
        return false;
    }

    key = (sf::Keyboard::Key)keyInt;
    return true;
}

void ConfigManager::WriteBool(std::vector<std::string>& lines, const char* itemName, bool& boolean)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << std::boolalpha << boolean;
    lines.push_back(tempOutput.str());
}

void ConfigManager::WriteInt(std::vector<std::string>& lines, const char* itemName, int& integer)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << integer;
    lines.push_back(tempOutput.str());
}

void ConfigManager::WriteKey(std::vector<std::string>& lines, const char* itemName, sf::Keyboard::Key& key)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << (int)key;
    lines.push_back(tempOutput.str());
}

// Loads in all the configuration values.
bool ConfigManager::LoadConfigurationValues(std::vector<std::string>& configFileLines)
{
    int lineCounter = 0;

    if (!LoadInt(configFileLines[lineCounter], ConfigVersion))
    {
        Logger::Log("Error decoding the configuration file version!");
        return false;
    }

    ++lineCounter;
    if (!LoadBool(configFileLines[lineCounter], IsFullscreen))
    {
        Logger::Log("Error decoding the fullscreen toggle!");
        return false;
    }

    ++lineCounter;
    if (!LoadInt(configFileLines[lineCounter], ScreenWidth))
    {
        Logger::Log("Error reading in the screen width!");
        return false;
    }

    ++lineCounter;
    if (!LoadInt(configFileLines[lineCounter], ScreenHeight))
    {
        Logger::Log("Error reading in the screen height!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ThrustForwardsKey))
    {
        Logger::Log("Error reading in the thrust forwards key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ThrustReverseKey))
    {
        Logger::Log("Error reading in the thrust reverse key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ThrustLeftKey))
    {
        Logger::Log("Error reading in the thrust left key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ThrustRightKey))
    {
        Logger::Log("Error reading in the thrust right key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ThrustUpKey))
    {
        Logger::Log("Error reading in the thrust up key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ThrustDownKey))
    {
        Logger::Log("Error reading in the thrust down key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], RotateLeftKey))
    {
        Logger::Log("Error reading in the rotate left key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], RotateRightKey))
    {
        Logger::Log("Error reading in the rotate right key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], RotateUpKey))
    {
        Logger::Log("Error reading in the rotate up key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], RotateDownKey))
    {
        Logger::Log("Error reading in the rotate down key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], RotateCWKey))
    {
        Logger::Log("Error reading in the rotate CW key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], RotateCCWKey))
    {
        Logger::Log("Error reading in the rotate CCW key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ToggleRotationDampeningKey))
    {
        Logger::Log("Error reading in the rotation dampening toggle key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], ToggleTranslationDampeningKey))
    {
        Logger::Log("Error reading in the translational dampening toggle key!");
        return false;
    }

    ++lineCounter;
    if (!LoadKey(configFileLines[lineCounter], PauseKey))
    {
        Logger::Log("Error reading in the pause key!");
        return false;
    }

    return true;
}

void ConfigManager::WriteConfigurationValues(std::vector<std::string>& configFileLines)
{
    WriteInt(configFileLines, "ConfigVersion", ConfigVersion);
    
    WriteBool(configFileLines, "FullScreen", IsFullscreen);
    WriteInt(configFileLines, "ScreenWidth", ScreenWidth);
    WriteInt(configFileLines, "ScreenHeight", ScreenHeight);

    WriteKey(configFileLines, "ThrustForwards", ThrustForwardsKey);
    WriteKey(configFileLines, "ThrustReverse", ThrustReverseKey);
    WriteKey(configFileLines, "ThrustLeft", ThrustLeftKey);
    WriteKey(configFileLines, "ThrustRight", ThrustRightKey);
    WriteKey(configFileLines, "ThrustUp", ThrustUpKey);
    WriteKey(configFileLines, "ThrustDown", ThrustDownKey);
    
    WriteKey(configFileLines, "RotateLeft", RotateLeftKey);
    WriteKey(configFileLines, "RotateRight", RotateRightKey);
    WriteKey(configFileLines, "RotateUp", RotateUpKey);
    WriteKey(configFileLines, "RotateDown", RotateDownKey);
    WriteKey(configFileLines, "RotateCW", RotateCWKey);
    WriteKey(configFileLines, "RotateCCW", RotateCCWKey);
    
    WriteKey(configFileLines, "ToggleRotationDampening", ToggleRotationDampeningKey);
    WriteKey(configFileLines, "ToggleTranslationDampening", ToggleTranslationDampeningKey);
    WriteKey(configFileLines, "Pause", PauseKey);
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
    std::vector<std::string> lines;
    WriteConfigurationValues(lines);

    for (unsigned int i = 0; i < lines.size(); i++)
    {
        if (commentLines.count(i) != 0)
        {
            // There's a comment line at this position, so add it in.
            lines.insert(lines.begin() + i, commentLines[i]);
        }
    }

    std::stringstream resultingFile;
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        resultingFile << lines[i] << StringUtils::Newline;
    }

    if (!StringUtils::WriteStringToFile(configFileName, resultingFile.str()))
    {
        Logger::Log("Couldn't write out the updated config file!");
        return false;
    }

    return true;
}