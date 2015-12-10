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
    static void AddPointSet(vmath::vec3 pointA, vmath::vec3 pointB, vmath::vec3 pointC, std::vector<colorBarycentricVertex>& vertices);
    static void AddTriangleFan(std::vector<vmath::vec3> arrangedPoints, vmath::vec3 centerPoint, std::vector<colorBarycentricVertex>& vertices);
    static std::vector<vmath::vec3> AddTriangleRing(float radius, float ringHeight, int ringPoints, std::vector<vmath::vec3>& lastRingPoints, std::vector<colorBarycentricVertex>& vertices);
    
    static std::vector<colorBarycentricVertex> GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize);
public:
    static std::vector<colorBarycentricVertex> GenerateSun();
    static std::vector<colorBarycentricVertex> GenerateSmallAsteroid();
    static std::vector<colorBarycentricVertex> GenerateMediumAsteroid();
    static std::vector<colorBarycentricVertex> GenerateLargeAsteroid();
};