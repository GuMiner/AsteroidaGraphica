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
    if (pointIdx == TOP_ID)
    {
        return vmath::vec3(0, 0, radius);
    }
    else if (pointIdx == BOTTOM_ID)
    {
        return vmath::vec3(0, 0, -radius);
    }

    // Points go CW when viewed top-down, so generate as-such.
    float ringRadius = Geometry::GetRingRadius(radius, ringHeight);
    float angle = pointIdx * ((2.0f * 3.14159f) / (float)totalPoints);
    return vmath::vec3(ringRadius * cos(angle), ringRadius * sin(angle), ringHeight);
}

// Adds a set of points with proper barycentric vertices to the point set, with test RGB coloration.
void Geometry::AddPointSet(int pointAId, int pointBId, int pointCId)
{
    spherePoints.push_back(pointAId);
    spherePoints.push_back(pointBId);
    spherePoints.push_back(pointCId);
}

// Adds in a triangle fan connecting the last ring points to the given point.
void Geometry::AddTriangleFan(int centerPointId)
{
    for (unsigned int i = 0; i < lastRingPoints.size(); i++)
    {
        unsigned int nextPoint = (i == lastRingPoints.size() - 1) ? 0 : i + 1;
        Geometry::AddPointSet(lastRingPoints[i], lastRingPoints[nextPoint], centerPointId);
    }
}

// Adds in a ring of triangles, returning the added points used when forming the ring.
std::vector<int> Geometry::AddTriangleRing(int ringId, int ringPointCount)
{
    std::vector<int> currentRingPoints;

    bool lastRingIteration = false;
    float fraction = (float)lastRingPoints.size() / (float)ringPointCount;
    if (lastRingPoints.size() > (unsigned int)ringPointCount)
    {
        fraction = 1.0f / fraction;
        lastRingIteration = true;
    }

    // Add in the implicit triangle that will be forgotten if not pre-included.
    int currentPointId = lastRingIteration ? lastRingPoints[0] : GetPointId(ringId, 0);
    int nextPointId = lastRingIteration ? GetPointId(ringId, 0) : lastRingPoints[0];
    int priorRingPointId = lastRingIteration ? GetPointId(ringId, ringPointCount - 1) : lastRingPoints[lastRingPoints.size() - 1];
    Geometry::AddPointSet(currentPointId, nextPointId, priorRingPointId);

    // Iterate over the array with the larger number of points to not miss implicit triangles.
    float currentFractionValue = 0.001f;
    int currentSmallerRingPoint = 0;

    int pointIterationCount = (lastRingIteration ? (int)lastRingPoints.size() : ringPointCount);
    for (int j = 0; j < pointIterationCount; j++)
    {
        int previousSmallerRingPoint = currentSmallerRingPoint;
        currentSmallerRingPoint = (int)currentFractionValue;

        int currentPointId = lastRingIteration ? lastRingPoints[j] : GetPointId(ringId, j);
        if (previousSmallerRingPoint != currentSmallerRingPoint)
        {
            // Generate an implicit triangle between the current point and other two ring points, because we skipped a step on the smaller ring.
            int nextPointId = lastRingIteration ? GetPointId(ringId, previousSmallerRingPoint) : lastRingPoints[previousSmallerRingPoint];
            int priorRingPointId = lastRingIteration ? GetPointId(ringId, currentSmallerRingPoint) : lastRingPoints[currentSmallerRingPoint];
            Geometry::AddPointSet(currentPointId, nextPointId, priorRingPointId);
        }

        // Generate a normal triangle given our current points.
        int nextIndex = (j == pointIterationCount - 1) ? 0 : j + 1;
        int nextPointId = lastRingIteration ? lastRingPoints[nextIndex] : GetPointId(ringId, nextIndex);
        int priorRingPointId = lastRingIteration ? GetPointId(ringId, currentSmallerRingPoint) : lastRingPoints[currentSmallerRingPoint];
        Geometry::AddPointSet(currentPointId, nextPointId, priorRingPointId);

        currentRingPoints.push_back(lastRingIteration ? priorRingPointId : currentPointId);
        currentFractionValue += fraction;
    }

    return currentRingPoints;
}

// Retrieves the ID of a point on the ring, creating a new one if it doesn't exist.
int Geometry::GetPointId(int ring, int ringPoint)
{
    if (spherePointLookup.count(ring) == 0)
    {
        // No points for this ring at all
        spherePointLookup[ring] = std::map<int, int>();
    }

    if (spherePointLookup[ring].count(ringPoint) == 0)
    {
        spherePointLookup[ring][ringPoint] = nextPointId;
        ++nextPointId;
    }

    return spherePointLookup[ring][ringPoint];
}

// Resets the class variables to generate a new spherical archetype.
void Geometry::Reset()
{
    nextPointId = 0;
    spherePointLookup.clear();
    lastRingPoints.clear();
    spherePoints.clear();
}

// Generates the trianges for a spherical shape with the specified major axis deformation, per-point deformation, and radius.
// Returns the number of new vertices added to the universal vertices list.
unsigned int Geometry::GenerateSphericalArchetype(universalVertices& universalVertices, float radius, float majorAxisDeformation, float perPointDeformation, float triangleSize)
{
    Reset();

    int ringCount = (int)vmath::max(1.0f, 2.0f * radius / triangleSize);
    float stepSize = (2.0f * radius) / (float)ringCount;
    
    int topId = GetPointId(TOP_ID, TOP_ID);
    
    float ringHeight = radius - stepSize;
	std::vector<int> ringCounts;
    for (int i = 0; i < ringCount - 1; i++)
    {
        // Find the number of points that will go on this ring.
        float ringCircumference = Geometry::GetRingCircumference(radius, ringHeight);
        int ringPointCount = (int)vmath::max(3.0f, ringCircumference / triangleSize);
		ringCounts.push_back(ringPointCount);

        if (i == 0)
        {
            // Special case -- this ring connects to the top point.
            for (int j = 0; j < ringPointCount; j++)
            {
                lastRingPoints.push_back(GetPointId(i + 1, j));
            }

            AddTriangleFan(topId);
        }
        else
        {
            // Normal ring, connecting the points to the points in the last ring and replacing the set of points there.
            lastRingPoints = Geometry::AddTriangleRing(i + 1, ringPointCount);

        }

        ringHeight -= stepSize;
    }

    // Connect the remaining point in the lastRingPoints array to the bottom point.
	int bottomId = GetPointId(BOTTOM_ID, BOTTOM_ID);
    AddTriangleFan(bottomId);

    // Generate a map of points for all our point IDs.
    // Note that the structure is Ring -> Ring Point -> Point ID.
    std::map<int, vmath::vec3> vertices;
    for (std::map<int, std::map<int, int>>::iterator iter = spherePointLookup.begin(); iter != spherePointLookup.end(); iter++)
    {
        for (std::map<int, int>::iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); innerIter++)
        {
            ringHeight = radius - (stepSize * (float)iter->first);
            float ringCircumference = Geometry::GetRingCircumference(radius, ringHeight);
            int ringPoints = (int)vmath::max(3.0f, ringCircumference / triangleSize);

            // [PointId] = current point position.
            vertices[innerIter->second] = Geometry::GetRingPointPosition(radius, ringHeight, innerIter->first, ringPoints);
        }
    }

    // Perform major axis and per-point deformation on the map of points.
    float axisDeformationAmount = 1.0f + majorAxisDeformation;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        // Note that the per-point deformation is passed in pre-randomized to filter the overall randomness, and this makes the randomness diverse overall.
        float xDeformation = Constants::Rand(perPointDeformation);
        float yDeformation = Constants::Rand(perPointDeformation);
        float zDeformation = Constants::Rand(perPointDeformation);
        
        vertices[i] = vmath::vec3(vertices[i][0] + xDeformation, vertices[i][1] + yDeformation, vertices[i][2] * axisDeformationAmount + zDeformation);
    }

    // Given our map of points and our large list of point IDs, write out the data in vertex-index format.
    int barycentricCounter = 0;
	int barycentricIndex = 0;
	int positionOffset = universalVertices.positions.size();
	bool barycentricLayer = false;
	bool firstToggle = true;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        vmath::vec3 point = vertices[i];

		// Generating the barycentric coordinates is rather tricky with indexed primitives. 
		if (i == 0)
		{
			// First point is a Z.
			universalVertices.barycentrics.push_back(vmath::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		}
		else if (i == vertices.size() - 1)
		{
			// True == XY. False == ZA. However, because we *just* swapped a layer, we use the old positioning.
			if (!barycentricLayer)
			{
				universalVertices.barycentrics.push_back(vmath::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			}
			else
			{
				universalVertices.barycentrics.push_back(vmath::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			}
		}
		else if (barycentricLayer)
		{
			if (firstToggle)
			{
				universalVertices.barycentrics.push_back(vmath::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			}
			else
			{
				universalVertices.barycentrics.push_back(vmath::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			}

			firstToggle = !firstToggle;
			++barycentricCounter;
			if (barycentricCounter == ringCounts[barycentricIndex])
			{
				++barycentricIndex;
				barycentricCounter = 0;
				barycentricLayer = !barycentricLayer;
			}
		}
		else
		{
			if (firstToggle)
			{
				universalVertices.barycentrics.push_back(vmath::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			}
			else
			{
				universalVertices.barycentrics.push_back(vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			}

			firstToggle = !firstToggle;
			++barycentricCounter;
			if (barycentricCounter == ringCounts[barycentricIndex])
			{
				++barycentricIndex;
				barycentricCounter = 0;
				barycentricLayer = !barycentricLayer;
			}
		}

		universalVertices.positions.push_back(point);
    }

	for (unsigned int i = 0; i < spherePoints.size(); i++)
	{
		// Note that there may already be vertices in the list of vertices, so we add in the indices with a position offset.
		universalVertices.indices.push_back(spherePoints[i] + positionOffset);
	}
    
	return vertices.size();
}

// Generates the sun, which is large.
unsigned int Geometry::GenerateSun(universalVertices& vertices)
{
    return GenerateSphericalArchetype(
		vertices,
        ConfigManager::SunSize,
        0.0f,
        Constants::Rand(ConfigManager::SunMaxPerPointDeformation),
        ConfigManager::SunTriangleSize);
}

// Generates a small asteroid, which means it is less than a grid size
unsigned int Geometry::GenerateSmallAsteroid(universalVertices& vertices)
{
    return GenerateSphericalArchetype(
		vertices,
        ConfigManager::SmallAsteroidSize + Constants::Rand(ConfigManager::SmallAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::SmallAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::SmallAsteroidSizeMaxPerPointDeformation),
        ConfigManager::SmallAsteroidTriangleSize);
}

// Generates a medium asteroid, which means it is about 40% of a grid size.
unsigned int Geometry::GenerateMediumAsteroid(universalVertices& vertices)
{
    return GenerateSphericalArchetype(
		vertices,
        ConfigManager::MediumAsteroidSize + Constants::Rand(ConfigManager::MediumAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::MediumAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::MediumAsteroidSizeMaxPerPointDeformation),
        ConfigManager::MediumAsteroidTriangleSize);
}

// Generates a large asteroid, which means it is about the size of the grid.
unsigned int Geometry::GenerateLargeAsteroid(universalVertices& vertices)
{
    return GenerateSphericalArchetype(
		vertices,
        ConfigManager::LargeAsteroidSize + Constants::Rand(ConfigManager::LargeAsteroidSizeMaxVariation),
        Constants::Rand(ConfigManager::LargeAsteroidSizeMaxAxisDeformation),
        Constants::Rand(ConfigManager::LargeAsteroidSizeMaxPerPointDeformation),
        ConfigManager::LargeAsteroidTriangleSize);
}