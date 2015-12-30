#include "Logger.h"
#include "KeyBindingConfig.h"

sf::Keyboard::Key KeyBindingConfig::ThrustForwardsKey;
sf::Keyboard::Key KeyBindingConfig::ThrustReverseKey;
sf::Keyboard::Key KeyBindingConfig::ThrustLeftKey;
sf::Keyboard::Key KeyBindingConfig::ThrustRightKey;
sf::Keyboard::Key KeyBindingConfig::ThrustUpKey;
sf::Keyboard::Key KeyBindingConfig::ThrustDownKey;

sf::Keyboard::Key KeyBindingConfig::RotateLeftKey;
sf::Keyboard::Key KeyBindingConfig::RotateRightKey;
sf::Keyboard::Key KeyBindingConfig::RotateUpKey;
sf::Keyboard::Key KeyBindingConfig::RotateDownKey;
sf::Keyboard::Key KeyBindingConfig::RotateCWKey;
sf::Keyboard::Key KeyBindingConfig::RotateCCWKey;

sf::Keyboard::Key KeyBindingConfig::ToggleRotationDampeningKey;
sf::Keyboard::Key KeyBindingConfig::ToggleTranslationDampeningKey;
sf::Keyboard::Key KeyBindingConfig::PauseKey;

bool KeyBindingConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
	LoadConfigurationValue(Key, ThrustForwardsKey, "Error reading in the thrust forwards key!");
	LoadConfigurationValue(Key, ThrustReverseKey, "Error reading in the thrust reverse key!");
	LoadConfigurationValue(Key, ThrustLeftKey, "Error reading in the thrust left key!");
	LoadConfigurationValue(Key, ThrustRightKey, "Error reading in the thrust right key!");
	LoadConfigurationValue(Key, ThrustUpKey, "Error reading in the thrust up key!");
	LoadConfigurationValue(Key, ThrustDownKey, "Error reading in the thrust down key!");
	LoadConfigurationValue(Key, RotateLeftKey, "Error reading in the rotate left key!");
	LoadConfigurationValue(Key, RotateRightKey, "Error reading in the rotate right key!");
	LoadConfigurationValue(Key, RotateUpKey, "Error reading in the rotate up key!");
	LoadConfigurationValue(Key, RotateDownKey, "Error reading in the rotate down key!");
	LoadConfigurationValue(Key, RotateCWKey, "Error reading in the rotate CW key!");
	LoadConfigurationValue(Key, RotateCCWKey, "Error reading in the rotate CCW key!");
	LoadConfigurationValue(Key, ToggleRotationDampeningKey, "Error reading in the rotation dampening toggle key!");
	LoadConfigurationValue(Key, ToggleTranslationDampeningKey, "Error reading in the translational dampening toggle key!");
	LoadConfigurationValue(Key, PauseKey, "Error reading in the pause key!");

	return true;
}

void KeyBindingConfig::WriteConfigValues()
{
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
}

KeyBindingConfig::KeyBindingConfig(const char* configName)
	: ConfigManager(configName)
{
}
