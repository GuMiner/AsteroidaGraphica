#include <vector>
#include "ConfigManager.h"
#include "Constants.h"
#include "Geometry.h"
#include "vmath.hpp"

// Generates the trianges for a spherical shape with the specified major axis deformation, per-point deformation, and radius.
colorBarycentricVertex* Geometry::GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize)
{
    int ringCount = (int)vmath::max(1.0f, (int)(radius / triangleSize));
    std::vector<colorBarycentricVertex> vertices;
    colorBarycentricVertex top;
    top.Set(0, 0, radius, 0, 1.0f, 0, 0, 0, 1.0f);

    colorBarycentricVertex bottom;
    bottom.Set(0, 0, -radius, 0, 0, 1.0f, 0, 0, 1.0f);

    // Add our first point, 
    vertices.push_back(top);

    std::vector<vmath::vec3> lastRingPoints;
    lastRingPoints.push_back(vmath::vec3(top.x, top.y, top.z));
    for (int i = 0; i < ringCount; i++)
    {
        if (ringCount == 0)
        {
            // Special case -- this ring connects to the top point.

        }
        else
        {
            // Normal ring, connecting the points to the points in the last ring and replacing the set of points there.
        }
    }

    // Connect the remaining point in the lastRingPoints array to the bottom point.

    vertices.push_back(bottom);

    // Perform major axis and per-point deformation.
    float deformationAmount = 1.0f + (majorAxisDeformation / radius);
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        // Note that the per-point deformation is passed in pre-randomized to filter the overall randomness, and this makes the randomness diverse overall.
        float xDeformation = Constants::Rand(perPointDeformation);
        float yDeformation = Constants::Rand(perPointDeformation);
        float zDeformation = Constants::Rand(perPointDeformation);

        vertices[i].Set(vertices[i].x + xDeformation, vertices[i].y + yDeformation, vertices[i].z * deformationAmount + zDeformation,
            vertices[i].r, vertices[i].g, vertices[i].b, vertices[i].xb, vertices[i].yb, vertices[i].zb);
    }

    colorBarycentricVertex *arrayVertices = new colorBarycentricVertex[vertices.size()];
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        // TODO use a more efficient copy mechanism here.
        arrayVertices[i] = vertices[i];
    }

    return arrayVertices;
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

// Generates a small asteroid, which means it is less than a grid size
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