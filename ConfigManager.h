#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML\Window.hpp>
#include "vmath.hpp"

// Function macro to wrap calls to Load* and spit out an error message on failure.
#define LoadConfigurationValue(Type, Variable, ErrorMessage) \
	currentLine = configFileLines[++lineCounter];			 \
	if (!Load##Type(##Variable))		  					 \
	{														 \
		Logger::Log(##ErrorMessage);                         \
		return false;										 \
	}														 \


// Loads in the configuration details for the rest of the system.
class ConfigManager
{
private:
	const char* configFileName;
	
	// Empty lines are counted as comment lines to preserve formatting of spacing.
	std::string CommentString;
	std::map<int, std::string> commentLines;

protected:
	std::string currentLine;
	int lineCounter;
    bool LoadBool(bool& boolean);
    bool LoadInt(int& integer);
    bool LoadFloat(float& floatingPoint);
    bool LoadKey(sf::Keyboard::Key& key);
	bool LoadVector(vmath::vec3& vector);

	std::vector<std::string> outputLines;
    void WriteBool(const char* itemName, bool& boolean);
    void WriteInt(const char* itemName, int& integer);
    void WriteFloat(const char* itemName, float& floatingPoint);
    void WriteKey(const char* itemName, sf::Keyboard::Key& key);
	void WriteVector(const char* itemName, vmath::vec3& vector);

	// Performs config-manager specific loading and writing. Called by read/write configuration.
    bool LoadConfigurationValues(std::vector<std::string>& lines);
    void WriteConfigurationValues();

	// Implemented by derived classes for loading / writing.
	virtual bool LoadConfigValues(std::vector<std::string>& lines) = 0;
	virtual void WriteConfigValues() = 0;

public:
	int configVersion;

    ConfigManager(const char* configFileName);
    bool ReadConfiguration();
    bool WriteConfiguration();
};

