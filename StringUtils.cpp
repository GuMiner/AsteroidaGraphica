#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "StringUtils.h"

// Loads a file as a really big string; returns true on success.
bool StringUtils::LoadStringFromFile(const std::string& filename, std::string& result)
{
    std::ifstream file(filename.c_str());
    if (!file)
    {
        return false;
    }

    std::stringstream stream;
    stream << file.rdbuf();
    result = stream.str();

    file.close();
    return true;
}

// Writes a really big string to a file; returns true on success.
bool StringUtils::WriteStringToFile(const std::string& filename, std::string& data)
{
    std::ofstream file(filename.c_str());
    if (!file)
    {
        return false;
    }
    
    file << data.c_str();
    file.close();

    return true;
}

// Splits a string into substrings, optionally including empty tokens if present.
void StringUtils::Split(const std::string& stringToSplit, char delimator, bool excludeEmpty, std::vector<std::string>& splitStrings)
{
    splitStrings.clear();
    std::stringstream stringStream(stringToSplit);

    std::string item;
    while (std::getline(stringStream, item, delimator))
    {
        if (!excludeEmpty || (excludeEmpty && !item.empty()))
        {
            splitStrings.push_back(item);
        }
    }
}

// Splits a line into two parts and grabs the number in the second part.
bool StringUtils::SplitAndGrabInt(const std::string& line, int& value)
{
    char SPACE = ' ';

    std::vector<std::string> stringParts;
    Split(line, SPACE, true, stringParts);

    if (stringParts.size() != 2)
    {
        return false;
    }
    if (!ParseIntFromString(stringParts[1], value))
    {
        return false;
    }
    return true;
}

// Determines if the given string starts with the provided sub string.
bool StringUtils::StartsWith(const std::string& givenString, const std::string& subString)
{
    if (givenString.length() < subString.length())
    {
        return false;
    }

    return (strncmp(givenString.substr(0, subString.length()).c_str(), subString.c_str(), subString.length()) == 0);
}

// Attempts to parse an integer from a string.
bool StringUtils::ParseIntFromString(const std::string& stringToParse, int& value)
{
    std::istringstream inputStream(stringToParse);
    return inputStream >> value ? true : false;
}

// Attempts to parse a double from a string.
bool StringUtils::ParseDoubleFromString(const std::string& stringToParse, double& value)
{
    std::istringstream inputStream(stringToParse);
    return inputStream >> value ? true : false;
}