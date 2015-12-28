#include "ConfigManager.h"
#include "Geometry.h"
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
	smallAsteroidVertexCounts = new GLuint[ConfigManager::SmallAsteroidTypes];
	mediumAsteroidVertexCounts = new GLuint[ConfigManager::MediumAsteroidTypes];
	largeAsteroidVertexCounts = new GLuint[ConfigManager::LargeAsteroidTypes];

	smallAsteroidOffsets = new GLuint[ConfigManager::SmallAsteroidTypes];
	mediumAsteroidOffsets = new GLuint[ConfigManager::MediumAsteroidTypes];
	largeAsteroidOffsets = new GLuint[ConfigManager::LargeAsteroidTypes];

	Geometry geometry;

	Logger::Log("Generating small asteroid types...");
	GLuint vertexCount = 0;
	for (int i = 0; i < ConfigManager::SmallAsteroidTypes; i++)
	{
		smallAsteroidVertexCounts[i] = geometry.GenerateSmallAsteroid(allAsteroids);
		smallAsteroidOffsets[i] = vertexCount;
		vertexCount += smallAsteroidVertexCounts[i];
	}

	Logger::Log("Generating medium asteroid types...");
	for (int i = 0; i < ConfigManager::MediumAsteroidTypes; i++)
	{
		mediumAsteroidVertexCounts[i] = geometry.GenerateMediumAsteroid(allAsteroids);
		mediumAsteroidOffsets[i] = vertexCount;
		vertexCount += mediumAsteroidVertexCounts[i];
	}

	Logger::Log("Generating large asteroid types...");
	for (int i = 0; i < ConfigManager::LargeAsteroidTypes; i++)
	{
		largeAsteroidVertexCounts[i] = geometry.GenerateLargeAsteroid(allAsteroids);
		largeAsteroidOffsets[i] = vertexCount;
		vertexCount += largeAsteroidVertexCounts[i];
	}
}

bool Asteroida::Initialize(ShaderManager& shaderManager, Elementa* elementa)
{
	if (!InitializeShader(shaderManager))
	{
		return false;
	}

	this->elementa = elementa;

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

    // Asteroid random generation with resource statistics.
	universalVertices allAsteroids;
	GenerateAsteroidTypes(allAsteroids);
    
	Logger::Log("Customizing resource stats");
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, asteroidOresTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F_ARB, ConfigManager::AsteroidCount);

	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		float isOre = Constants::Rand();
		if (isOre > ConfigManager::OreEmptyRatio)
		{
			// No ore
			oreColors.push_back(vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			asteroidElement.push_back(Elementa::Elements::None);
			elementAmounts.push_back(0.0f);
		}
		else
		{
			// Some ore.
			Elementa::Elements element = elementa->GetRandomElement();
			vmath::vec3 oreColor = elementa->GetOreColor(element);

			float elementAmount = elementa->GetRandomOreAmount(element);
			float maxElementAmount = elementa->GetMaxRandomOreAmount(element);
			
			oreColors.push_back(vmath::vec4(oreColor[0], oreColor[1], oreColor[2], elementAmount / maxElementAmount));
			asteroidElement.push_back(element);
			elementAmounts.push_back(elementAmount);
		}
	}

	// TODO make asteroids mineable, which means this will update on a semi-regular basis.
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, ConfigManager::AsteroidCount, GL_RGBA, GL_FLOAT, &oreColors[0]);

	Logger::Log("Customizing rotations...");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, asteroidRotationTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F_ARB, ConfigManager::AsteroidCount);
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		vmath::vec3 randomAxis = vmath::normalize(vmath::vec3(Constants::Rand(), Constants::Rand(), Constants::Rand()));
		float randomAngle = Constants::Rand() * 2 * 3.15159f;

		vmath::vec3 currentEulerRotation = vmath::vec3(
			Constants::Rand(ConfigManager::AsteroidRotationSpeed),
			Constants::Rand(ConfigManager::AsteroidRotationSpeed),
			Constants::Rand(ConfigManager::AsteroidRotationSpeed));

		rotations.push_back(vmath::quaternion::fromAxisAngle(randomAngle, randomAxis));
		eulerRotations.push_back(currentEulerRotation);
	}

	Logger::Log("Customizing colors...");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, asteroidColorTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB32F_ARB, ConfigManager::AsteroidCount);
	for (int i = 0; i < ConfigManager::AsteroidCount; i++)
	{
		colors.push_back(Paletta::GetRandomAsteroidColor());
	}

	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, ConfigManager::AsteroidCount, GL_RGB, GL_FLOAT, &colors[0]);

	Logger::Log("Customizing positions...");
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, asteroidPositionTexture);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F_ARB, ConfigManager::AsteroidCount);
	
    // TODO Randomly position the asteroids in the appropriate zones, moving at orbital speed.
    float separation = 2.0f;
    
    int fracAsteroidCount = (int)sqrt(ConfigManager::AsteroidCount);
	int xP = 15;
	int yP = 15;
    for (int i = 0; i < ConfigManager::AsteroidCount; i++)
    {
		positions.push_back(vmath::vec4(xP * separation, yP * separation, 0.0f, 0.0f));
		velocities.push_back(vmath::vec3(-400, 400, 0));

		++xP;
		if (xP == 100)
		{
			xP = 15;
			++yP;
		}
    }

	updatedAsteroidPositionAndRotation = true;

	Logger::Log("Adding ID field for asteroids");
	for (unsigned int i = 0; i < (unsigned int)ConfigManager::SmallAsteroidTypes; i++)
	{
		for (unsigned int j = 0; j < smallAsteroidVertexCounts[i]; j++)
		{
			allAsteroids.ids.push_back(i);
		}

		masses.push_back(ConfigManager::SmallAsteroidMass);
	}

	for (unsigned int i = 0; i < (unsigned int)ConfigManager::MediumAsteroidTypes; i++)
	{
		for (unsigned int j = 0; j < mediumAsteroidVertexCounts[i]; j++)
		{
			allAsteroids.ids.push_back(i + ConfigManager::SmallAsteroidTypes);
		}

		masses.push_back(ConfigManager::MediumAsteroidMass);
	}

	for (unsigned int i = 0; i < (unsigned int)ConfigManager::LargeAsteroidTypes; i++)
	{
		for (unsigned int j = 0; j < largeAsteroidVertexCounts[i]; j++)
		{
			allAsteroids.ids.push_back(i + ConfigManager::SmallAsteroidTypes + ConfigManager::MediumAsteroidTypes);
		}

		masses.push_back(ConfigManager::LargeAsteroidMass);
	}

	Logger::Log("Sending asteroid data to the GPU...");
	universalVertices::TransferToOpenGl(allAsteroids, positionBuffer, 0, barycentricBuffer, 0, idBuffer, indicesBuffer);
	asteroidIndexCount = allAsteroids.indices.size();

    return true;
}

// Renders the asteroids with the given perspective/look-at projection matrix.
void Asteroida::Render(vmath::mat4& projectionMatrix)
{
	if (updatedAsteroidPositionAndRotation)
	{
		updateMutex.lock();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, asteroidPositionTexture);
		glTexSubImage1D(GL_TEXTURE_1D, 0, 0, ConfigManager::AsteroidCount, GL_RGBA, GL_FLOAT, &positions[0]);
		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, asteroidRotationTexture);
		glTexSubImage1D(GL_TEXTURE_1D, 0, 0, ConfigManager::AsteroidCount, GL_RGBA, GL_FLOAT, &rotations[0]);
		updateMutex.unlock();
		
		updatedAsteroidPositionAndRotation = false;
	}

    glUseProgram(asteroidShaderProgram);
	glUniform1i(asteroidPositionLocation, 0);
	glUniform1i(asteroidColorsLocation, 1);
	glUniform1i(asteroidRotationsLocation, 2);
	glUniform1i(asteroidOresLocation, 3);
	
    glBindVertexArray(vao);
		
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
    vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mv_matrix);
    
	glDrawElements(GL_TRIANGLES, asteroidIndexCount, GL_UNSIGNED_INT, nullptr);
}

Asteroida::~Asteroida()
{
    delete[] smallAsteroidVertexCounts;
    delete[] mediumAsteroidVertexCounts;
    delete[] largeAsteroidVertexCounts;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &barycentricBuffer);
	glDeleteBuffers(1, &idBuffer);
	glDeleteBuffers(1, &indicesBuffer);

    glDeleteTextures(1, &asteroidPositionTexture);
	glDeleteTextures(1, &asteroidColorTexture);
	glDeleteTextures(1, &asteroidRotationTexture);
}
