#include "ConfigManager.h"
#include "Logger.h"
#include "Paletta.h"
#include "Asteroida.h"

Asteroida::Asteroida()
{
}

// Initializes the asteroid shader.
bool Asteroida::InitializeShader(ShaderManager& shaderManager)
{
	Logger::Log("Asteroida shader creation...");
	if (!shaderManager.CreateShaderProgram("asteroidRender", &asteroidShaderProgram))
	{
		return false;
	}

	if (!shaderManager.CreateShaderProgram("asteroidPointRender", &pointRenderShaderProgram))
	{
		return false;
	}

	Logger::Log("Asteroida shader creation successful!");

	perspectiveLocation = glGetUniformLocation(asteroidShaderProgram, "perspectiveMatrix");
	viewMatrixLocation = glGetUniformLocation(asteroidShaderProgram, "viewMatrix");
	asteroidPositionLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidPositions");
	asteroidColorsLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidColors");
	asteroidRotationsLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidRotations");
	asteroidOresLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidOres");

	pointRenderProjLocation = glGetUniformLocation(pointRenderShaderProgram, "proj_matrix");
	return true;
}

// Generates the asteroid archetypes for the small, medium, and large asteroids, storing them (in that order) in the universal vertices.
void Asteroida::GenerateAsteroidTypes(universalVertices& allAsteroids)
{
	Geometry geometry;

	Logger::Log("Generating small asteroid types...");
	unsigned int vertexCounter = 0;
	unsigned int indiciesCounter = 0;
	ArchetypeData archetype;
	for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
	{
		archetype.vertexOffset = vertexCounter;
		archetype.indexOffset = indiciesCounter;

		geometry.GenerateSmallAsteroid(allAsteroids);
		
		archetype.indexCount = allAsteroids.indices.size() - indiciesCounter;
		archetype.vertexCount = allAsteroids.positions.size() - vertexCounter;
		archetypeData.push_back(archetype);

		indiciesCounter = allAsteroids.indices.size();
		vertexCounter = allAsteroids.positions.size();
	}

	Logger::Log("Generating medium asteroid types...");
	for (int i = 0; i < ConfigManager::MediumAsteroidTypes; i++)
	{
		archetype.vertexOffset = vertexCounter;
		archetype.indexOffset = indiciesCounter;

		geometry.GenerateMediumAsteroid(allAsteroids);

		archetype.indexCount = allAsteroids.indices.size() - indiciesCounter;
		archetype.vertexCount = allAsteroids.positions.size() - vertexCounter;
		archetypeData.push_back(archetype);

		indiciesCounter = allAsteroids.indices.size();
		vertexCounter = allAsteroids.positions.size();
	}

	Logger::Log("Generating large asteroid types...");
	for (int i = 0; i < ConfigManager::LargeAsteroidTypes; i++)
	{
		archetype.vertexOffset = vertexCounter;
		archetype.indexOffset = indiciesCounter;

		geometry.GenerateLargeAsteroid(allAsteroids);

		archetype.indexCount = allAsteroids.indices.size() - indiciesCounter;
		archetype.vertexCount = allAsteroids.positions.size() - vertexCounter;
		archetypeData.push_back(archetype);

		indiciesCounter = allAsteroids.indices.size();
		vertexCounter = allAsteroids.positions.size();
	}
}

// Returns the orbital velocity for a position away from the sun, heading CW in the XY plane.
vmath::vec3 Asteroida::GetOrbitalVelocity(const vmath::vec2& pos) const
{
	float length = vmath::length(pos);
	float orbitalSpeed = sqrt(ConfigManager::GravitationalConstant * ConfigManager::SolarMass / length);
	return vmath::normalize(vmath::cross(vmath::vec3(-pos[0], -pos[1], 0), vmath::vec3(0, 0, 1))) * orbitalSpeed;
}

// Generates random color and rotations for all the asteroids.
void Asteroida::GenerateRandomColorRotations()
{
	// Give all the asteroids a random color.
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		asteroids.colors.push_back(Paletta::GetRandomAsteroidColor());
	}

	// Give all the asteroids a random rotation.
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		vmath::vec3 randomAxis = vmath::normalize(vmath::vec3(Constants::Rand(), Constants::Rand(), Constants::Rand()));
		float randomAngle = Constants::Rand() * 2 * 3.15159f;

		vmath::vec3 currentEulerRotation = vmath::vec3(
			Constants::Rand(ConfigManager::AsteroidRotationSpeed),
			Constants::Rand(ConfigManager::AsteroidRotationSpeed),
			Constants::Rand(ConfigManager::AsteroidRotationSpeed));

		asteroids.rotations.push_back(vmath::quaternion::fromAxisAngle(randomAngle, randomAxis));
		asteroids.eulerRotations.push_back(currentEulerRotation);
	}
}

// Generates the ore distribution of all the asteroids.
void Asteroida::GenerateAsteroidOreDistribution()
{
	// Generate the ore distribution.
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		float isOre = Constants::Rand();
		if (isOre > ConfigManager::OreEmptyRatio)
		{
			// No ore
			asteroids.oreColors.push_back(vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			asteroids.asteroidElement.push_back(Elementa::Elements::None);
			asteroids.elementAmounts.push_back(0.0f);
		}
		else
		{
			// Some ore.
			Elementa::Elements element = elementa->GetRandomElement();
			vmath::vec3 oreColor = elementa->GetOreColor(element);

			float elementAmount = elementa->GetRandomOreAmount(element);
			float maxElementAmount = elementa->GetMaxRandomOreAmount(element);

			asteroids.oreColors.push_back(vmath::vec4(oreColor[0], oreColor[1], oreColor[2], elementAmount / maxElementAmount));
			asteroids.asteroidElement.push_back(element);
			asteroids.elementAmounts.push_back(elementAmount);
		}
	}
}

// Generates the field of all the asteroids.
void Asteroida::GenerateAsteroidField()
{
	// Determine if this is a S/M/L asteroid and the archetype ID.
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		Geometry::AsteroidSize asteroidSize = (Geometry::AsteroidSize)Constants::Rand(0, 3);
		asteroids.asteroidSizes.push_back(asteroidSize);
		asteroids.archetypeIds.push_back(Constants::Rand(0, archetypeData.size()));

		switch (asteroidSize)
		{
		case Geometry::AsteroidSize::Small:
			asteroids.masses.push_back(ConfigManager::SmallAsteroidMass);
			break;
		case Geometry::AsteroidSize::Medium:
			asteroids.masses.push_back(ConfigManager::MediumAsteroidMass);
			break;
		case Geometry::AsteroidSize::Large:
			asteroids.masses.push_back(ConfigManager::LargeAsteroidMass);
			break;
		}
	}

	// Give all the asteroids a random position, ensuring all asteroids spawn within the shields.
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		float distance = ConfigManager::AsteroidTorusMinDistance + ConfigManager::LargeAsteroidSize * 2
			 + Constants::Rand() * (ConfigManager::AsteroidTorusRadius - 4 * ConfigManager::LargeAsteroidSize);
		float angle = 2 * 3.14159f * Constants::Rand();
		float height = Constants::Rand(ConfigManager::AsteroidTorusHeight);

		distance -= ConfigManager::AsteroidTorusMinDistance * (1.0f - cos(atan(height / ConfigManager::AsteroidTorusMinDistance)));

		vmath::vec4 position = vmath::vec4(distance * cos(angle), distance * sin(angle), height, 0.0f);
		asteroids.positions.push_back(position);
		asteroids.velocities.push_back(GetOrbitalVelocity(vmath::vec2(position[0], position[1])));
	}

	Logger::Log("Generating random colors and rotations...");
	GenerateRandomColorRotations();
	
	Logger::Log("Generating asteroid ore distribution...");
	GenerateAsteroidOreDistribution();
}

// Generates the GPU data that stores the visible asteroids in the asteroid field.
void Asteroida::GenerateAsteroidView()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &barycentricBuffer);
	glGenBuffers(1, &idBuffer);
	glGenBuffers(1, &indicesBuffer);

	glGenTextures(1, &asteroidPositionTexture);
	glGenTextures(1, &asteroidColorTexture);
	glGenTextures(1, &asteroidRotationTexture);
	glGenTextures(1, &asteroidOresTexture);

	Logger::Log("Creating 1D texture for ore visualization...");
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, asteroidOresTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F_ARB, ConfigManager::AsteroidRenderLimit);

	Logger::Log("Creating 1D texture for asteroid rotation...");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, asteroidRotationTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F_ARB, ConfigManager::AsteroidRenderLimit);

	Logger::Log("Creating 1D texture for asteroid color visualization...");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, asteroidColorTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB32F_ARB, ConfigManager::AsteroidRenderLimit);

	Logger::Log("Creating 1D texture for asteroid positioning...");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, asteroidPositionTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F_ARB, ConfigManager::AsteroidRenderLimit);

	// Generate data for point rendering.
	glGenVertexArrays(1, &pointRenderVao);
	glBindVertexArray(pointRenderVao);

	glGenBuffers(1, &pointRenderPositionBuffer);
	glGenBuffers(1, &pointRenderColorBuffer);
}

bool Asteroida::Initialize(ShaderManager& shaderManager, Elementa* elementa)
{
	if (!InitializeShader(shaderManager))
	{
		return false;
	}

	this->elementa = elementa;
	
    // Asteroid archetypes, field, and view generation.
	GenerateAsteroidTypes(allAsteroids);
    
	Logger::Log("Generating the asteroid field...");
	GenerateAsteroidField();

	Logger::Log("Generating the asteroid view...");
	GenerateAsteroidView();

    return true;
}

// Updates the visible asteroids and sends the vertex and texture data to the gpu.
void Asteroida::UpdateVisibleAsteroids(const vmath::vec3& shipPosition)
{
	// Clear the results from the last update.
	gpuAsteroids.Reset();
	gpuPositions.clear();
	gpuColors.clear();
	gpuRotations.clear();
	gpuOreColors.clear();

	int currentIndex = 0;
	int asteroidCount = 0;
	
	// Yes, this should be in the class. Considering this is essentially debugging functionality, I'm leaving it as-is.
	static bool hitRenderLimitBefore = false;
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		if (asteroidCount == ConfigManager::AsteroidRenderLimit)
		{
			if (!hitRenderLimitBefore)
			{
				Logger::Log("Warning! Hit the asteroid render limit -- consider increasing the limit or decreasing your view distance");
				hitRenderLimitBefore = true;
			}
			break;
		}

		const vmath::vec4 asteroidPosition = asteroids.positions[i];
		if (!(asteroidPosition[0] - ConfigManager::ViewDistance < shipPosition[0] && asteroidPosition[0] + ConfigManager::ViewDistance > shipPosition[0]))
		{
			// Asteroid not in x-bounds
			continue;
		}
		if (!(asteroidPosition[1] - ConfigManager::ViewDistance < shipPosition[1] && asteroidPosition[1] + ConfigManager::ViewDistance > shipPosition[1]))
		{
			// Asteroid not in y-bounds
			continue;
		}
		if (!(asteroidPosition[2] - ConfigManager::ViewDistance < shipPosition[2] && asteroidPosition[2] + ConfigManager::ViewDistance > shipPosition[2]))
		{
			// Asteroid not in z-bounds
			continue;
		}

		// Load the vertex data
		int vertexOffset = archetypeData[asteroids.archetypeIds[i]].vertexOffset;
		int vertexCount = archetypeData[asteroids.archetypeIds[i]].vertexCount;
		int indiciesOffset = archetypeData[asteroids.archetypeIds[i]].indexOffset;
		int indiciesCount = archetypeData[asteroids.archetypeIds[i]].indexCount;

		for (int j = 0; j < vertexCount; j++)
		{
			gpuAsteroids.positions.push_back(allAsteroids.positions[vertexOffset + j]);
			gpuAsteroids.barycentrics.push_back(allAsteroids.barycentrics[vertexOffset + j]);
			gpuAsteroids.ids.push_back(asteroidCount); // Yes, asteroidCount. This is the ID of the asteroid, not the vertex.
		}

		for (int j = 0; j < indiciesCount; j++)
		{
			// This remaps the indices to the new VBO.
			gpuAsteroids.indices.push_back(currentIndex + (allAsteroids.indices[indiciesOffset + j] - vertexOffset));
		}

		// Load the texture data.
		gpuPositions.push_back(asteroids.positions[i]);
		gpuColors.push_back(asteroids.colors[i]);
		gpuRotations.push_back(asteroids.rotations[i]);
		gpuOreColors.push_back(asteroids.oreColors[i]);

		// Note that the *value* of each index cannot be > the new vertex count.
		currentIndex += vertexCount;
		++asteroidCount;
	}

	// Send the Vertex data
	glBindVertexArray(vao);
	gpuAsteroids.TransferPositionToOpenGl(positionBuffer);
	gpuAsteroids.TransferBarycentricsToOpenGl(barycentricBuffer);
	gpuAsteroids.TransferIdsToOpenGl(idBuffer);
	gpuAsteroids.TransferIndicesToOpenGl(indicesBuffer);

	// Send the Texture data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, asteroidPositionTexture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, gpuPositions.size(), GL_RGBA, GL_FLOAT, &gpuPositions[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, asteroidColorTexture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, gpuColors.size(), GL_RGB, GL_FLOAT, &gpuColors[0]);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, asteroidRotationTexture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, gpuRotations.size(), GL_RGBA, GL_FLOAT, &gpuRotations[0]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, asteroidOresTexture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, gpuOreColors.size(), GL_RGBA, GL_FLOAT, &gpuOreColors[0]);

	// Update the points matrix.
	glBindVertexArray(pointRenderVao);
	universalVertices::TransferDirectToOpenGl(asteroids.positions, pointRenderPositionBuffer, asteroids.colors, pointRenderColorBuffer);
}

// Renders the asteroids with the given perspective/look-at projection matrix.
void Asteroida::Render(const vmath::mat4& perspectiveMatrix, const vmath::mat4& viewMatrix, const vmath::vec3& shipPosition)
{
	UpdateVisibleAsteroids(shipPosition);

    glUseProgram(asteroidShaderProgram);
	glUniform1i(asteroidPositionLocation, 0);
	glUniform1i(asteroidColorsLocation, 1);
	glUniform1i(asteroidRotationsLocation, 2);
	glUniform1i(asteroidOresLocation, 3);
	
	// Asteroids that made the cut.
    glBindVertexArray(vao);
    glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, perspectiveMatrix);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, viewMatrix);
	glDrawElements(GL_TRIANGLES, gpuAsteroids.indices.size(), GL_UNSIGNED_INT, nullptr);

	// Points for all asteroids that missed the cut.
	glUseProgram(pointRenderShaderProgram);
	glBindVertexArray(pointRenderVao);
	glUniformMatrix4fv(pointRenderProjLocation, 1, GL_FALSE, perspectiveMatrix * viewMatrix);
	glDrawArrays(GL_POINTS, 0, allAsteroids.positions.size());

}

Asteroida::~Asteroida()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &barycentricBuffer);
	glDeleteBuffers(1, &idBuffer);
	glDeleteBuffers(1, &indicesBuffer);

    glDeleteTextures(1, &asteroidPositionTexture);
	glDeleteTextures(1, &asteroidColorTexture);
	glDeleteTextures(1, &asteroidRotationTexture);

	glDeleteVertexArrays(1, &pointRenderVao);
	glDeleteBuffers(1, &pointRenderPositionBuffer);
	glDeleteBuffers(1, &pointRenderColorBuffer);
}
