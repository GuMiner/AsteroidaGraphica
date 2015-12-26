#include "ConfigManager.h"
#include "Constants.h"
#include "Paletta.h"

Paletta::Paletta()
{
}

// Retrieves a random astroid color from the gradient.
vmath::vec3 Paletta::GetRandomAsteroidColor()
{
	float randomValue = Constants::Rand();
	return (ConfigManager::AsteroidGradientEndColor - ConfigManager::AsteroidGradientStartColor) * randomValue + ConfigManager::AsteroidGradientStartColor;
}

Paletta::~Paletta()
{
}
