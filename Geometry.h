#pragma once
#include "Vertex.h"

// Generates in-game geometry so that asteroids are *always* different per game but *always* consistent once generated.
class Geometry
{
    colorBarycentricVertex* GenerateSphericalArchetype(float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize);
public:
    colorBarycentricVertex* GenerateSun();
    colorBarycentricVertex* GenerateSmallAsteroid();
    colorBarycentricVertex* GenerateMediumAsteroid();
    colorBarycentricVertex* GenerateLargeAsteroid();
};

