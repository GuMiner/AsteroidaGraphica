#pragma once
#include <map>
#include <vector>
#include "Vertex.h"
#include "vmath.hpp"

// Generates in-game geometry so that asteroids are *always* different per game but *always* consistent once generated.
class Geometry
{
    const static int TOP_ID = -1;
    const static int BOTTOM_ID = -2;

    // Ring -> Ring Point (angle) -> Point ID
    std::map<int, std::map<int, int>> spherePointLookup;
    int nextPointId;
    
    // Point IDs of the last ring.
    std::vector<int> lastRingPoints;

    // Point IDs making up the sphere.
    std::vector<int> spherePoints;

    static float GetRingRadius(float radius, float ringHeight);
    static float GetRingCircumference(float radius, float ringHeight);
    static vmath::vec3 Geometry::GetRingPointPosition(float radius, float ringHeight, int pointIdx, int totalPoints);

    void AddPointSet(int pointAId, int pointBId, int pointCId);
    void AddTriangleFan(int centerPointId);
    std::vector<int> AddTriangleRing(int ringId, int ringPointCount);
    
    int GetPointId(int ring, int ringPoint);
    void Reset();
    std::vector<barycentricVertex> GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize);
public:
    std::vector<barycentricVertex> GenerateSun();
    std::vector<barycentricVertex> GenerateSmallAsteroid();
    std::vector<barycentricVertex> GenerateMediumAsteroid();
    std::vector<barycentricVertex> GenerateLargeAsteroid();
};