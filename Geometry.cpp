#include "ConfigManager.h"
#include "Constants.h"
#include "Geometry.h"

// Determines the ring radius.
float Geometry::GetRingRadius(float radius, float ringHeight)
{
    return sin(acos(ringHeight / radius)) * radius;
}

// Determines the ring circumference 
float Geometry::GetRingCircumference(float radius, float ringHeight)
{
    // You can draw out the triangle within the sphere to determine this equation.
    return 2.0f * 3.14159f * Geometry::GetRingRadius(radius, ringHeight);
}

// Determines the position of a point on the ring in 3D space. Supports positive and negative indexing.
vmath::vec3 Geometry::GetRingPointPosition(float radius, float ringHeight, int pointIdx, int totalPoints)
{
    // Points go CW when viewed top-down, so generate as-such.
    float ringRadius = Geometry::GetRingRadius(radius, ringHeight);
    float angle = pointIdx * ((2.0f * 3.14159f) / (float)totalPoints);
    return vmath::vec3(ringRadius * cos(angle), ringRadius * sin(angle), ringHeight);
}

// Generates the trianges for a spherical shape with the specified major axis deformation, per-point deformation, and radius.
std::vector<colorBarycentricVertex> Geometry::GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize)
{
    int ringCount = (int)vmath::max(1.0f, 2.0f * radius / triangleSize);
    float stepSize = (2.0f * radius) / (float)ringCount;

    std::vector<colorBarycentricVertex> vertices;
    colorBarycentricVertex top;
    top.Set(0, 0, radius, 0, 1.0f, 0, 0, 0, 1.0f);

    colorBarycentricVertex bottom;
    bottom.Set(0, 0, -radius, 0, 0, 1.0f, 0, 0, 1.0f);

    // Add our first point, 
    vertices.push_back(top);

    std::vector<vmath::vec3> lastRingPoints;
    lastRingPoints.push_back(vmath::vec3(top.x, top.y, top.z));

    float ringHeight = radius - stepSize;
    for (int i = 0; i < ringCount; i++)
    {
        // Find the number of points that will go on this ring.
        float ringCircumference = Geometry::GetRingCircumference(radius, ringHeight);
        int ringPoints = (int)vmath::max(3.0f, ringCircumference / triangleSize);

        for (int j = 0; j < ringPoints; j++)
        {
            if (i == 0)
            {
                // Special case -- this ring connects to the top point.
                vmath::vec3 currentPoint = Geometry::GetRingPointPosition(radius, ringHeight, j, ringPoints);
                vmath::vec3 nextPoint = Geometry::GetRingPointPosition(radius, ringHeight, j + 1, ringPoints);

                // Form a triangle with the given points, given that the top point has a barycentric vertex of 1.0f;
                colorBarycentricVertex point;
                point.Set(currentPoint[0], currentPoint[1], currentPoint[2], 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
                vertices.push_back(point);
                
                point.Set(nextPoint[0], nextPoint[1], nextPoint[2], 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
                vertices.push_back(point);

                vertices.push_back(top);
            }
            else
            {
                // Normal ring, connecting the points to the points in the last ring and replacing the set of points there.
            }
        }

        ringHeight -= stepSize;
    }

    // Connect the remaining point in the lastRingPoints array to the bottom point.

    // vertices.push_back(bottom);

    // TODO TEST CODE
    //vertices.clear();
    /*colorBarycentricVertex point;
    point.Set(0, 0, 0, 1, 1, 1, 0, 0, 1);
    vertices.push_back(point);
    point.Set(0, 5, 0, 0, 1, 0, 0, 1, 0);
    vertices.push_back(point);
    point.Set(5, 0, 0, 1, 0, 0, 1, 0, 0);
    vertices.push_back(point);
    point.Set(0, 5, 0, 0, 1, 0, 0, 0, 1);
    vertices.push_back(point);
    point.Set(0, 0, 5, 0, 0, 1, 0, 1, 0);
    vertices.push_back(point);
    point.Set(0, 0, 0, 1, 1, 1, 1, 0, 0);
    vertices.push_back(point);
    point.Set(5, 0, 0, 1, 0, 0, 0, 0, 1);
    vertices.push_back(point);
    point.Set(0, 0, 5, 0, 0, 1, 0, 1, 0);
    vertices.push_back(point);
    point.Set(0, 0, 0, 1, 1, 1, 1, 0, 0);
    vertices.push_back(point);
    point.Set(5, 0, 0, 1, 0, 0, 0, 0, 1);
    vertices.push_back(point);
    point.Set(0, 0, 5, 0, 0, 1, 0, 1, 0);
    vertices.push_back(point);
    point.Set(0, 5, 0, 0, 1, 0, 1, 0, 0);
    vertices.push_back(point);*/

    // Perform major axis and per-point deformation.
    /* TODO reinstate deformation when we can do it correctly and have the drawing all correct.
    
    float deformationAmount = 1.0f + (majorAxisDeformation / radius);
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        // Note that the per-point deformation is passed in pre-randomized to filter the overall randomness, and this makes the randomness diverse overall.
        float xDeformation = Constants::Rand(perPointDeformation);
        float yDeformation = Constants::Rand(perPointDeformation);
        float zDeformation = Constants::Rand(perPointDeformation);

        vertices[i].Set(vertices[i].x + xDeformation, vertices[i].y + yDeformation, vertices[i].z * deformationAmount + zDeformation,
            vertices[i].r, vertices[i].g, vertices[i].b, vertices[i].xb, vertices[i].yb, vertices[i].zb);
    }*/
    
    return vertices;
}

// Generates the sun, which is large.
std::vector<colorBarycentricVertex> Geometry::GenerateSun()
{
    return GenerateSphericalArchetype(
        ConfigManager::SunSize,
        0.0f,
        Constants::Rand(ConfigManager::SunMaxPerPointDeformation),
        ConfigManager::SunTriangleSize);
}

// Generates a small asteroid, which means it is less than a grid size
std::vector<colorBarycentricVertex> Geometry::GenerateSmallAsteroid()
{
    return GenerateSphericalArchetype(
        ConfigManager::SmallAsteroidSize + Constants::Rand(ConfigManager::SmallAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::SmallAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::SmallAsteroidSizeMaxPerPointDeformation),
        ConfigManager::SmallAsteroidTriangleSize);
}

// Generates a medium asteroid, which means it is about 40% of a grid size.
std::vector<colorBarycentricVertex> Geometry::GenerateMediumAsteroid()
{
    return GenerateSphericalArchetype(
        ConfigManager::MediumAsteroidSize + Constants::Rand(ConfigManager::MediumAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::MediumAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::MediumAsteroidSizeMaxPerPointDeformation),
        ConfigManager::MediumAsteroidTriangleSize);
}

// Generates a large asteroid, which means it is about the size of the grid.
std::vector<colorBarycentricVertex> Geometry::GenerateLargeAsteroid()
{
    return GenerateSphericalArchetype(
        ConfigManager::LargeAsteroidSize + Constants::Rand(ConfigManager::LargeAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::LargeAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::LargeAsteroidSizeMaxPerPointDeformation),
        ConfigManager::LargeAsteroidTriangleSize);
}