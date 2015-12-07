#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML\Window.hpp>

// Loads in the configuration details for the rest of the system.
class ConfigManager
{
    const char* configFileName;
    
    // Empty lines are counted as comment lines to preserve formatting of spacing.
    std::string CommentString;
    std::map<int, std::string> commentLines;

    bool LoadBool(std::string& line, bool& boolean);
    bool LoadInt(std::string& line, int& integer);
    bool LoadFloat(std::string& line, float& floatingPoint);
    bool LoadKey(std::string& line, sf::Keyboard::Key& key);

    void WriteBool(std::vector<std::string>& lines, const char* itemName, bool& boolean);
    void WriteInt(std::vector<std::string>& lines, const char* itemName, int& integer);
    void WriteFloat(std::vector<std::string>& lines, const char* itemName, float& floatingPoint);
    void WriteKey(std::vector<std::string>& lines, const char* itemName, sf::Keyboard::Key& key);

    bool LoadConfigurationValues(std::vector<std::string>& lines);
    void WriteConfigurationValues(std::vector<std::string>& lines);

public:
    static int ConfigVersion;

    // Graphics
    static bool IsFullscreen;
    static int ScreenWidth;
    static int ScreenHeight;

    // Key bindings
    static sf::Keyboard::Key ThrustForwardsKey;
    static sf::Keyboard::Key ThrustReverseKey;
    static sf::Keyboard::Key ThrustLeftKey;
    static sf::Keyboard::Key ThrustRightKey;
    static sf::Keyboard::Key ThrustUpKey;
    static sf::Keyboard::Key ThrustDownKey;

    static sf::Keyboard::Key RotateLeftKey;
    static sf::Keyboard::Key RotateRightKey;
    static sf::Keyboard::Key RotateUpKey;
    static sf::Keyboard::Key RotateDownKey;
    static sf::Keyboard::Key RotateCWKey;
    static sf::Keyboard::Key RotateCCWKey;

    static sf::Keyboard::Key ToggleRotationDampeningKey;
    static sf::Keyboard::Key ToggleTranslationDampeningKey;
    static sf::Keyboard::Key PauseKey;

    // World Generation
    static float SunSize;
    static float SunMaxPerPointDeformation;
    static float SunTriangleSize;

    static float SmallAsteroidSize;
    static float SmallAsteroidSizeMaxVariation;
    static float SmallAsteroidSizeMaxAxisDeformation;
    static float SmallAsteroidSizeMaxPerPointDeformation;
    static float SmallAsteroidTriangleSize;

    static float MediumAsteroidSize;
    static float MediumAsteroidSizeMaxVariation;
    static float MediumAsteroidSizeMaxAxisDeformation;
    static float MediumAsteroidSizeMaxPerPointDeformation;
    static float MediumAsteroidTriangleSize;

    static float LargeAsteroidSize;
    static float LargeAsteroidSizeMaxVariation;
    static float LargeAsteroidSizeMaxAxisDeformation;
    static float LargeAsteroidSizeMaxPerPointDeformation;
    static float LargeAsteroidTriangleSize;

    ConfigManager();
    bool ReadConfiguration();
    bool WriteConfiguration();
};

