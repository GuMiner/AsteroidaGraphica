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

    bool LoadConfigurationValues(std::vector<std::string>& lines);
    bool WriteConfigurationValues(std::vector<std::string>& lines);

public:
    static int configVersion;

    static bool isFullscreen;
    static int screenWidth;
    static int screenHeight;

    static sf::Keyboard::Key thrustForwardsKey;
    static sf::Keyboard::Key thrustReverseKey;
    static sf::Keyboard::Key thrustLeftKey;
    static sf::Keyboard::Key thrustRightKey;
    static sf::Keyboard::Key thrustUpKey;
    static sf::Keyboard::Key thrustDownKey;

    static sf::Keyboard::Key rotateLeftKey;
    static sf::Keyboard::Key rotateRightKey;
    static sf::Keyboard::Key rotateUpKey;
    static sf::Keyboard::Key rotateDownKey;
    static sf::Keyboard::Key rotateCWKey;
    static sf::Keyboard::Key rotateCCWKey;

    static sf::Keyboard::Key toggleRotationDampeningKey;
    static sf::Keyboard::Key toggleTranslationDampeningKey;
    static sf::Keyboard::Key pauseKey;

    ConfigManager();
    bool ReadConfiguration();
    bool WriteConfiguration();
};

