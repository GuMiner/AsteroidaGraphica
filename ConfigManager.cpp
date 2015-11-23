#include <sstream>
#include "Logger.h"
#include "StringUtils.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager()
{
    CommentString = "#";

    configFileName = "config/config.txt";
}

bool ConfigManager::LoadConfigurationValues(std::vector<std::string>& configFileLines)
{
    // TODO implement
    return true;
}

bool ConfigManager::WriteConfigurationValues(std::vector<std::string>& configFileLines)
{
    // TODO implement
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
    StringUtils::Split(entireFile, StringUtils::Newline, true, lines);
    for (std::vector<std::string>::iterator iterator = lines.begin(); iterator != lines.end(); iterator++)
    {
        if (StringUtils::StartsWith(*iterator, CommentString))
        {
            commentLines[currentLine] = std::string(*iterator);

            iterator = lines.erase(iterator);
            --iterator;
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
