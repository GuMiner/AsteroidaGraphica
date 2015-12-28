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

	Logger::Log("Asteroida shader creation successful!");

	mvLocation = glGetUniformLocation(asteroidShaderProgram, "mv_matrix");
	projLocation = glGetUniformLocation(asteroidShaderProgram, "proj_matrix");
	asteroidPositionLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidPositions");
	asteroidColorsLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidColors");
	asteroidRotationsLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidRotations");
	asteroidOresLocation = glGetUniformLocation(asteroidShaderProgram, "asteroidOres");

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

	// Give all the asteroids a random position.
	int fracAsteroidCount = (int)sqrt(ConfigManager::AsteroidCount);
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		float distance = 50.0f + Constants::Rand() * 30.0f; // TODO put this in config manager also.
		float angle = 2 * 3.14159f * Constants::Rand();
		float height = Constants::Rand(2.0f); // TODO this should also be in the config.

		vmath::vec4 position = vmath::vec4(distance * cos(angle), distance * sin(angle), height, 0.0f);
		asteroids.positions.push_back(position);
		asteroids.velocities.push_back(GetOrbitalVelocity(vmath::vec2(position[0], position[1])));
	}

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
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F_ARB, ConfigManager::AsteroidCount);
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
	// TODO select the visible asteroids, not the first 100 asteroids
	
	// Clear the results from the last update.
	gpuAsteroids.Reset();
	gpuPositions.clear();
	gpuColors.clear();
	gpuRotations.clear();
	gpuOreColors.clear();

	int currentIndex = 0;
	for (int i = 0; i < ConfigManager::AsteroidRenderLimit; i++)
	{
		// Load the vertex data
		int vertexOffset = archetypeData[asteroids.archetypeIds[i]].vertexOffset;
		int vertexCount = archetypeData[asteroids.archetypeIds[i]].vertexCount;
		int indiciesOffset = archetypeData[asteroids.archetypeIds[i]].indexOffset;
		int indiciesCount = archetypeData[asteroids.archetypeIds[i]].indexCount;

		for (int j = 0; j < vertexCount; j++)
		{
			gpuAsteroids.positions.push_back(allAsteroids.positions[vertexOffset + j]);
			gpuAsteroids.barycentrics.push_back(allAsteroids.barycentrics[vertexOffset + j]);
			gpuAsteroids.ids.push_back(i); // Yes, i. This is the ID of the asteroid, not the vertex.
		}

		for (int j = 0; j < indiciesCount; j++)
		{
			// This remaps the indices to the new VBO.
			gpuAsteroids.indices.push_back(currentIndex + (allAsteroids.indices[indiciesOffset + j] - vertexOffset));
		}

		// Note that the *value* of each index cannot be > the new vertex count.
		currentIndex += vertexCount;

		// Load the texture data.
		gpuPositions.push_back(asteroids.positions[i]);
		gpuColors.push_back(asteroids.colors[i]);
		gpuRotations.push_back(asteroids.rotations[i]);
		gpuOreColors.push_back(asteroids.oreColors[i]);
	}

	// Send the Vertex data
	universalVertices::TransferToOpenGl(gpuAsteroids, positionBuffer, 0, barycentricBuffer, 0, idBuffer, indicesBuffer);

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
}

// Renders the asteroids with the given perspective/look-at projection matrix.
void Asteroida::Render(const vmath::mat4& projectionMatrix, const vmath::vec3& shipPosition)
{
	glBindVertexArray(vao);
	UpdateVisibleAsteroids(shipPosition);

    glUseProgram(asteroidShaderProgram);
	glUniform1i(asteroidPositionLocation, 0);
	glUniform1i(asteroidColorsLocation, 1);
	glUniform1i(asteroidRotationsLocation, 2);
	glUniform1i(asteroidOresLocation, 3);
	
    glBindVertexArray(vao);
		
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
    vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
    
	glDrawElements(GL_TRIANGLES, gpuAsteroids.indices.size(), GL_UNSIGNED_INT, nullptr);
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
}
