#pragma once
#include "Vertex.h"

class Geometry
{
public:
    Geometry();
    colorBarycentricVertex* GenerateSmallAsteroid();
    colorBarycentricVertex* GenerateMediumAsteroid();
    colorBarycentricVertex* GenerateLargeAsteroid();
    ~Geometry();
};

