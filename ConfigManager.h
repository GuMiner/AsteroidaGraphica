#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML\Window.hpp>
#include "vmath.hpp"

// Loads in the configuration details for the rest of the system.
class ConfigManager
{
    const char* configFileName;
    
    // Empty lines are counted as comment lines to preserve formatting of spacing.
    std::string CommentString;
    std::map<int, std::string> commentLines;

	std::string currentLine;
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

    bool LoadConfigurationValues(std::vector<std::string>& lines);
    void WriteConfigurationValues();

public:
    static int ConfigVersion;

    // Graphics
    static bool IsFullscreen;
    static int ScreenWidth;
    static int ScreenHeight;

	static int TextImageSize;

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

    static int SmallAsteroidTypes;
    static int MediumAsteroidTypes;
    static int LargeAsteroidTypes;

    static int AsteroidCount;

	// Physics
	static int PhysicsThreadDelay;

	static float BaseShipMass;
	static float SmallAsteroidMass;
	static float MediumAsteroidMass;
	static float LargeAsteroidMass;

	static float SolarMass;
	static float GravitationalConstant;

	static float AsteroidTimestep;

	static float AsteroidRotationSpeed;

	// Palettes
	static vmath::vec3 AsteroidGradientStartColor;
	static vmath::vec3 AsteroidGradientEndColor;

	static vmath::vec3 WaterOreColor;
	static vmath::vec3 FeOreColor;
	static vmath::vec3 SiOreColor;
	static vmath::vec3 CuOreColor;
	static vmath::vec3 UOreColor;
	static vmath::vec3 AuOreColor;
	static vmath::vec3 PtOreColor;
	static vmath::vec3 ImpOreColor;

	// Resources
	static float BaseWaterOre;
	static float MaxWaterOre;
	static float BaseFeOre;
	static float MaxFeOre;
	static float BaseSiOre;
	static float MaxSiOre;
	static float BaseCuOre;
	static float MaxCuOre;
	static float BaseUOre;
	static float MaxUOre;
	static float BaseAuOre;
	static float MaxAuOre;
	static float BasePtOre;
	static float MaxPtOre;
	static float BaseImpOre;
	static float MaxImpOre;

    ConfigManager();
    bool ReadConfiguration();
    bool WriteConfiguration();
};

