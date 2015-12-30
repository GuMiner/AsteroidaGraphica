#pragma once
#include <SFML\Window.hpp>
#include "ConfigManager.h"

class KeyBindingConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();

public:
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

	KeyBindingConfig(const char* configName);
};

