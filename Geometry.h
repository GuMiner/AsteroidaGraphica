#pragma once
#include <vector>
#include "Vertex.h"
#include "vmath.hpp"

// Generates in-game geometry so that asteroids are *always* different per game but *always* consistent once generated.
class Geometry
{
    static float GetRingRadius(float radius, float ringHeight);
    static float GetRingCircumference(float radius, float ringHeight);
    static vmath::vec3 Geometry::GetRingPointPosition(float radius, float ringHeight, int pointIdx, int totalPoints);
    static std::vector<colorBarycentricVertex> GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize);
public:
    static std::vector<colorBarycentricVertex> GenerateSun();
    static std::vector<colorBarycentricVertex> GenerateSmallAsteroid();
    static std::vector<colorBarycentricVertex> GenerateMediumAsteroid();
    static std::vector<colorBarycentricVertex> GenerateLargeAsteroid();
};