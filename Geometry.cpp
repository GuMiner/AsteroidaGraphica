#include "ConfigManager.h"
#include "Constants.h"
#include "Geometry.h"

// Generates the trianges for a spherical shape with the specified major axis deformation, per-point deformation, and radius.
colorBarycentricVertex* Geometry::GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize)
{
    return nullptr;
}

// Generates the sun, which is large.
colorBarycentricVertex* Geometry::GenerateSun()
{
    return GenerateSphericalArchetype(
        ConfigManager::SunSize,
        0.0f,
        Constants::Rand(ConfigManager::SunMaxPerPointDeformation),
        ConfigManager::SunTriangleSize);
}

// Generates a small asteroid, which means it is less than a grid size (~0.1 units large)
colorBarycentricVertex* Geometry::GenerateSmallAsteroid()
{
    return GenerateSphericalArchetype(
        ConfigManager::SmallAsteroidSize + Constants::Rand(ConfigManager::SmallAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::SmallAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::SmallAsteroidSizeMaxPerPointDeformation),
        ConfigManager::SmallAsteroidTriangleSize);
}

// Generates a medium asteroid, which means it is about 40% of a grid size.
colorBarycentricVertex* Geometry::GenerateMediumAsteroid()
{
    return GenerateSphericalArchetype(
        ConfigManager::MediumAsteroidSize + Constants::Rand(ConfigManager::MediumAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::MediumAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::MediumAsteroidSizeMaxPerPointDeformation),
        ConfigManager::MediumAsteroidTriangleSize);
}

// Generates a large asteroid, which means it is about the size of the grid.
colorBarycentricVertex* Geometry::GenerateLargeAsteroid()
{
    return GenerateSphericalArchetype(
        ConfigManager::LargeAsteroidSize + Constants::Rand(ConfigManager::LargeAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::LargeAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::LargeAsteroidSizeMaxPerPointDeformation),
        ConfigManager::LargeAsteroidTriangleSize);
}