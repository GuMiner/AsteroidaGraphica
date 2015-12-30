#include "Constants.h"
#include "Paletta.h"
#include "OreConfig.h"

Paletta::Paletta()
{
}

// Retrieves a random astroid color from the gradient.
vmath::vec3 Paletta::GetRandomAsteroidColor()
{
	float randomValue = Constants::Rand();
	return (OreConfig::AsteroidGradientEndColor - OreConfig::AsteroidGradientStartColor) * randomValue + OreConfig::AsteroidGradientStartColor;
}

Paletta::~Paletta()
{
}
