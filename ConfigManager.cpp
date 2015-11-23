#include <sstream>
#include "Logger.h"
#include "StringUtils.h"
#include "ConfigManager.h"

int ConfigManager::configVersion;

bool ConfigManager::isFullscreen;
int ConfigManager::screenWidth;
int ConfigManager::screenHeight;

sf::Keyboard::Key ConfigManager::thrustForwardsKey;
sf::Keyboard::Key ConfigManager::thrustReverseKey;
sf::Keyboard::Key ConfigManager::thrustLeftKey;
sf::Keyboard::Key ConfigManager::thrustRightKey;
sf::Keyboard::Key ConfigManager::thrustUpKey;
sf::Keyboard::Key ConfigManager::thrustDownKey;

sf::Keyboard::Key ConfigManager::rotateLeftKey;
sf::Keyboard::Key ConfigManager::rotateRightKey;
sf::Keyboard::Key ConfigManager::rotateUpKey;
sf::Keyboard::Key ConfigManager::rotateDownKey;
sf::Keyboard::Key ConfigManager::rotateCWKey;
sf::Keyboard::Key ConfigManager::rotateCCWKey;

sf::Keyboard::Key ConfigManager::toggleRotationDampeningKey;
sf::Keyboard::Key ConfigManager::toggleTranslationDampeningKey;
sf::Keyboard::Key ConfigManager::pauseKey;

ConfigManager::ConfigManager()
{
    CommentString = "#";
    configFileName = "config/config.txt";
}

bool ConfigManager::LoadConfigurationValues(std::vector<std::string>& configFileLines)
{
    int lineCounter = 0;

    std::string tempInput;
    if (!StringUtils::SplitAndGrabSecondary(configFileLines[lineCounter], tempInput) || !StringUtils::ParseIntFromString(tempInput, configVersion))
    {
        Logger::Log("Error decoding the configuration file version!");
        return false;
    }

    ++lineCounter;
    if (!StringUtils::SplitAndGrabSecondary(configFileLines[lineCounter], tempInput) || !StringUtils::ParseBoolFromString(tempInput, isFullscreen))
    {
        Logger::Log("Error decoding the fullscreen toggle!");
        return false;
    }

    return true;
}

bool ConfigManager::WriteConfigurationValues(std::vector<std::string>& configFileLines)
{
    std::stringstream tempOutput;
    tempOutput << "ConfigVersion" << StringUtils::Space << configVersion;
    configFileLines.push_back(tempOutput.str());

    tempOutput.str("");
    tempOutput << "FullScreen" << StringUtils::Space << (isFullscreen ? "true" : "false");
    configFileLines.push_back(tempOutput.str());

    return true;
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

    if (!WriteConfigurationValues(lines))
    {
        Logger::Log("Couldn't parse out the current configuration values!");
        return false;
    }

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
