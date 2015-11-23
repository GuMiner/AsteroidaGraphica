#pragma once

#include <string>
#include <vector>

class StringUtils
{
public:
    static const char Newline = '\n';

    // Writes a really big string to a file; returns true on success.
    static bool WriteStringToFile(const std::string& filename, std::string& data);

    // Loads a file as a really big string; returns true on success.
    static bool LoadStringFromFile(const std::string& filename, std::string& result);

    // Splits a string into substrings, optionally including empty tokens if present.
    static void Split(const std::string& stringToSplit, char delimator, bool excludeEmpty, std::vector<std::string>& splitStrings);

    // Splits a line into two parts and grabs the number in the second part.
    static bool SplitAndGrabInt(const std::string& line, int& value);

    // Determines if the given string starts with the provided sub string.
    static bool StartsWith(const std::string& givenString, const std::string& subString);

    // Attempts to parse an integer from a string.
    static bool ParseIntFromString(const std::string& stringToParse, int& value);

    // Attempts to parse a double from a string.
    static bool ParseDoubleFromString(const std::string& stringToParse, double& value);
};