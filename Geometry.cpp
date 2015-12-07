#include "ConfigManager.h"
#include "Constants.h"
#include "Geometry.h"

// Generates the trianges for a spherical shape with the specified major axis deformation, per-point deformation, and radius.
colorBarycentricVertex* Geometry::GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation)
{
    return nullptr;
}

// TODO all values should be configuration-modifiable

colorBarycentricVertex* Geometry::GenerateSun()
{
    return nullptr;
}

// Generates a small asteroid, which means it is less than a grid size (~0.1 units large)
colorBarycentricVertex* Geometry::GenerateSmallAsteroid()
{
    float size = 0.1f;
    float sizeVariation = 0.05f;
    float maxDeformation = 0.05f;
    float maxPerPointDeformation = 0.02f;

    float majorAxisDeformation = Constants::Rand(maxDeformation);
    float perPointDeformation = Constants::Rand(maxPerPointDeformation);
    return GenerateSphericalArchetype(size + Constants::Rand(sizeVariation), majorAxisDeformation, perPointDeformation);
}

// Generates a medium asteroid, which means it is about 40% of a grid size.
colorBarycentricVertex* Geometry::GenerateMediumAsteroid()
{
    float size = 0.4f;
    float sizeVariation = 0.2f;
    float maxDeformation = 0.1f;
    float maxPerPointDeformation = 0.05f;

    float majorAxisDeformation = Constants::Rand(maxDeformation);
    float perPointDeformation = Constants::Rand(maxPerPointDeformation);
    return GenerateSphericalArchetype(size + Constants::Rand(sizeVariation), majorAxisDeformation, perPointDeformation);
}

// Generates a large asteroid, which means it is about the size of the grid.
colorBarycentricVertex* Geometry::GenerateLargeAsteroid()
{
    float size = 1.0f;
    float sizeVariation = 0.2f;
    float maxDeformation = 0.2f;
    float maxPerPointDeformation = 0.1f;

    float majorAxisDeformation = Constants::Rand(maxDeformation);
    float perPointDeformation = Constants::Rand(maxPerPointDeformation);
    return GenerateSphericalArchetype(size + Constants::Rand(sizeVariation), majorAxisDeformation, perPointDeformation);
}