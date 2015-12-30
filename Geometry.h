#pragma once
#include <map>
#include <vector>
#include "Vertex.h"
#include "vmath.hpp"

// Generates in-game geometry so that asteroids are *always* different per game but *always* consistent once generated.
class Geometry
{
public:
	// Current asteroid size, used within Asteroida.
	enum AsteroidSize
	{
		Small,
		Medium,
		Large,
	};

private:
    const static int TOP_ID = -1;
    const static int BOTTOM_ID = 232323; // This should be fixed -- we're depending on the proper ordering of these within the map for barycentrics.
	// Note that the ID doesn't matter, but that illustrates the problem -- we're depending on the wrong ordering for proper barycentric rendering.

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
    unsigned int GenerateSphericalArchetype(universalVertices& vertices, float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize);
public:
    unsigned int GenerateSun(universalVertices& vertices);
	unsigned int GenerateForceField(universalVertices& vertices, float radius);
    unsigned int GenerateSmallAsteroid(universalVertices& vertices);
    unsigned int GenerateMediumAsteroid(universalVertices& vertices);
    unsigned int GenerateLargeAsteroid(universalVertices& vertices);
};