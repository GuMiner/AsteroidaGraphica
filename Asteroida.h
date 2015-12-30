#pragma once
#include <SFML\System.hpp>
#include <glbinding/gl/gl.h>
#include "Elementa.h"
#include "Geometry.h"
#include "ShaderManager.h"
#include "Vertex.h"
#include "vmath.hpp"

using namespace gl;

// Convenience structure to hold the asteroid data
struct Asteroids
{
	// Data that can be sent to the GPU.
	std::vector<vmath::vec4> positions;
	std::vector<vmath::vec3> colors;
	std::vector<vmath::quaternion> rotations;
	std::vector<vmath::vec4> oreColors;
	
	std::vector<Geometry::AsteroidSize> asteroidSizes;
	std::vector<int> archetypeIds;

	// Physical data
	std::vector<float> masses;
	std::vector<vmath::vec3> eulerRotations;
	std::vector<vmath::vec3> velocities;

	// Resource data
	std::vector<Elementa::Elements> asteroidElement;
	std::vector<float> elementAmounts;
};

// Convenience structure to hold asteroid archetype data
struct ArchetypeData
{
	unsigned int vertexCount;
	unsigned int vertexOffset;

	unsigned int indexCount;
	unsigned int indexOffset;
};

// Manages the asteroids that are part of this game.
class Asteroida
{
	Elementa* elementa;

	// GPU data
	GLuint asteroidShaderProgram;
    GLuint perspectiveLocation, viewMatrixLocation;
	GLuint asteroidPositionLocation, asteroidColorsLocation, asteroidRotationsLocation, asteroidOresLocation;

	GLuint pointRenderShaderProgram;
	GLuint pointRenderProjLocation;

    GLuint vao;
    GLuint positionBuffer;
	GLuint barycentricBuffer;
	GLuint idBuffer;
	GLuint indicesBuffer;

	GLuint pointRenderVao;
	GLuint pointRenderPositionBuffer;
	GLuint pointRenderColorBuffer;
	    
	GLuint asteroidPositionTexture;
	GLuint asteroidColorTexture;
	GLuint asteroidRotationTexture;
	GLuint asteroidOresTexture;

	// Holds all the asteroid archetypes.
	universalVertices allAsteroids;
	std::vector<ArchetypeData> archetypeData;

	// Holds the data that is sent to the GPU of the currently-visible asteroids.
	std::vector<vmath::vec4> gpuPositions;
	std::vector<vmath::vec3> gpuColors;
	std::vector<vmath::quaternion> gpuRotations;
	std::vector<vmath::vec4> gpuOreColors;
	universalVertices gpuAsteroids;

	vmath::vec3 GetOrbitalVelocity(const vmath::vec2& pos) const;

	bool InitializeShader(ShaderManager& shaderManager);
	void GenerateAsteroidTypes(universalVertices& allAsteroids);
	
	void GenerateRandomColorRotations();
	void GenerateAsteroidField();
	void GenerateAsteroidView();

	void UpdateVisibleAsteroids(const vmath::vec3& shipPosition);

public:
	Asteroids asteroids;

    Asteroida();
    bool Initialize(ShaderManager& shaderManager, Elementa* elementa);
    void Render(const vmath::mat4& perspectiveMatrix, const vmath::mat4& viewMatrix, const vmath::vec3& shipPosition);
    ~Asteroida();
};

