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

// Adds a set of points with proper barycentric vertices to the point set, with test RGB coloration.
void Geometry::AddPointSet(vmath::vec3 pointA, vmath::vec3 pointB, vmath::vec3 pointC, std::vector<colorBarycentricVertex>& vertices)
{
    colorBarycentricVertex point;
    point.Set(pointA[0], pointA[1], pointA[2], 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    vertices.push_back(point);

    point.Set(pointB[0], pointB[1], pointB[2], 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    vertices.push_back(point);

    point.Set(pointC[0], pointC[1], pointC[2], 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    vertices.push_back(point);
}

// Adds in a triangle fan connecting the arranged points to the center point.
void Geometry::AddTriangleFan(std::vector<vmath::vec3> arrangedPoints, vmath::vec3 centerPoint, std::vector<colorBarycentricVertex>& vertices)
{
    for (unsigned int i = 0; i < arrangedPoints.size(); i++)
    {
        unsigned int nextPoint = (i == arrangedPoints.size() - 1) ? 0 : i + 1;
        Geometry::AddPointSet(arrangedPoints[i], arrangedPoints[nextPoint], centerPoint, vertices);
    }
}

// Adds in a ring of triangles, returning the added points used when forming the ring.
std::vector<vmath::vec3> Geometry::AddTriangleRing(float radius, float ringHeight, int ringPoints, std::vector<vmath::vec3>& lastRingPoints, std::vector<colorBarycentricVertex>& vertices)
{
    std::vector<vmath::vec3> currentRingPoints;

    bool lastRingIteration = false;
    float fraction = (float)lastRingPoints.size() / (float)ringPoints;
    if (lastRingPoints.size() > (unsigned int)ringPoints)
    {
        fraction = 1.0f / fraction;
        lastRingIteration = true;
    }

    // Add in the implicit triangle that will be forgotten if not pre-included.
    vmath::vec3 currentPoint = lastRingIteration ? lastRingPoints[0] : Geometry::GetRingPointPosition(radius, ringHeight, 0, ringPoints);
    vmath::vec3 nextPoint = lastRingIteration ? Geometry::GetRingPointPosition(radius, ringHeight, 0, ringPoints) : lastRingPoints[0];
    vmath::vec3 priorRingPoint = lastRingIteration ? Geometry::GetRingPointPosition(radius, ringHeight, -1, ringPoints) : lastRingPoints[lastRingPoints.size() - 1];
    Geometry::AddPointSet(currentPoint, nextPoint, priorRingPoint, vertices);

    // Iterate over the array with the larger number of points to not miss implicit triangles.
    float currentFractionValue = 0.001f;
    int currentSmallerRingPoint = 0;

    int pointIterationCount = (lastRingIteration ? (int)lastRingPoints.size() : ringPoints);
    for (int j = 0; j < pointIterationCount; j++)
    {
        int previousSmallerRingPoint = currentSmallerRingPoint;
        currentSmallerRingPoint = (int)currentFractionValue;

        vmath::vec3 currentPoint = lastRingIteration ? lastRingPoints[j] : Geometry::GetRingPointPosition(radius, ringHeight, j, ringPoints);
        if (previousSmallerRingPoint != currentSmallerRingPoint)
        {
            // Generate an implicit triangle between the current point and other two ring points, because we skipped a step on the smaller ring.
            vmath::vec3 nextPoint = lastRingIteration ? Geometry::GetRingPointPosition(radius, ringHeight, previousSmallerRingPoint, ringPoints) : lastRingPoints[previousSmallerRingPoint];
            vmath::vec3 priorRingPoint = lastRingIteration ? Geometry::GetRingPointPosition(radius, ringHeight, currentSmallerRingPoint, ringPoints) : lastRingPoints[currentSmallerRingPoint];
            Geometry::AddPointSet(currentPoint, nextPoint, priorRingPoint, vertices);
        }

        // Generate a normal triangle given our current points.
        int nextIndex = (j == pointIterationCount - 1) ? 0 : j + 1;
        vmath::vec3 nextPoint = lastRingIteration ? lastRingPoints[nextIndex] : Geometry::GetRingPointPosition(radius, ringHeight, nextIndex, ringPoints);
        vmath::vec3 priorRingPoint = lastRingIteration ? Geometry::GetRingPointPosition(radius, ringHeight, currentSmallerRingPoint, ringPoints) : lastRingPoints[currentSmallerRingPoint];
        Geometry::AddPointSet(currentPoint, nextPoint, priorRingPoint, vertices);

        currentRingPoints.push_back(lastRingIteration ? priorRingPoint : currentPoint);
        currentFractionValue += fraction;
    }

    return currentRingPoints;
}

// Generates the trianges for a spherical shape with the specified major axis deformation, per-point deformation, and radius.
std::vector<colorBarycentricVertex> Geometry::GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize)
{
    int ringCount = (int)vmath::max(1.0f, 2.0f * radius / triangleSize);
    float stepSize = (2.0f * radius) / (float)ringCount;

    std::vector<colorBarycentricVertex> vertices;  
    std::vector<vmath::vec3> lastRingPoints;

    float ringHeight = radius - stepSize;
    for (int i = 0; i < ringCount; i++)
    {
        // Find the number of points that will go on this ring.
        float ringCircumference = Geometry::GetRingCircumference(radius, ringHeight);
        int ringPoints = (int)vmath::max(3.0f, ringCircumference / triangleSize);
        
        std::vector<vmath::vec3> lastPoints = lastRingPoints;
        lastRingPoints.clear();
        if (i == 0)
        {
            // Special case -- this ring connects to the top point.
            for (int j = 0; j < ringPoints; j++)
            {
                vmath::vec3 currentPoint = Geometry::GetRingPointPosition(radius, ringHeight, j, ringPoints);
                lastRingPoints.push_back(currentPoint);
            }

            vmath::vec3 top(0, 0, radius);
            Geometry::AddTriangleFan(lastRingPoints, top, vertices);
        }
        else
        {
            // Normal ring, connecting the points to the points in the last ring and replacing the set of points there.
            lastRingPoints = Geometry::AddTriangleRing(radius, ringHeight, ringPoints, lastPoints, vertices);
        }

        ringHeight -= stepSize;
    }

    // Connect the remaining point in the lastRingPoints array to the bottom point.
    vmath::vec3 bottom(0, 0, -radius);
    Geometry::AddTriangleFan(lastRingPoints, bottom, vertices);

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