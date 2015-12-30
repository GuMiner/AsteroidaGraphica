#include <thread>
#include "Physica.h"
#include "PhysicsConfig.h"

Physica::Physica()
{
    isAlive = true;
    isPaused = false;
    initialized = false;
}

void Physica::Initialize(Asteroida *asteroida, Stellaria *stellaria, Shipia *shipia)
{
    this->asteroida = asteroida;
	this->stellaria = stellaria;
	this->shipia = shipia;
    initialized = true;
}

// Calculates the acceleration applied to objects as part of the sun.
vmath::vec3 Physica::SunAcceleration(const vmath::vec3& pos) const
{
	// F_a = m_a*a
	// F_a = (m_s*m_a*G_c/(r_2))r_hat

	float distance = vmath::length(pos);
	float constants = -PhysicsConfig::SolarMass * PhysicsConfig::GravitationalConstant / (distance * distance);

	return constants * vmath::normalize(pos);
}

// Calculates the acceleration applied to the ship
vmath::vec3 Physica::ShipAcceleration(const vmath::vec3& pos) const
{
	return (shipia->shipForce / shipia->GetShipMass()) + SunAcceleration(pos);
}

// Performs RK4 step evaluation within the context of solar mass.
PosVel Physica::RK4SunStepEvaluate(const PosVel& initialState, float dt, const PosVel& derivative) const
{
	PosVel result;
	result.pos = initialState.vel + derivative.vel * dt;
	result.vel = SunAcceleration(initialState.pos + derivative.pos * dt);
	return result;
}

// Performs RK4 step evaluation within the context of solar mass for the ship.
PosVel Physica::RK4ShipStepEvaluate(const PosVel& initialState, float dt, const PosVel& derivative) const
{
	PosVel result;
	result.pos = initialState.vel + derivative.vel * dt;
	result.vel = ShipAcceleration(initialState.pos + derivative.pos * dt);
	return result;
}

// Moves an item based on it's position, velocity, and timestep.
void Physica::SunIntegration(vmath::vec3& pos, vmath::vec3& vel, float dt) const
{
	PosVel initialState;
	initialState.pos = pos;
	initialState.vel = vel;

	PosVel zero;
	zero.pos = vmath::vec3(0.0f, 0.0f, 0.0f);
	zero.vel = vmath::vec3(0.0f, 0.0f, 0.0f);
	
	PosVel left = RK4SunStepEvaluate(initialState, 0.0f, zero);
	PosVel middleLeft = RK4SunStepEvaluate(initialState, dt * 0.5f, left);
	PosVel middleRight = RK4SunStepEvaluate(initialState, dt * 0.5f, middleLeft);
	PosVel right = RK4SunStepEvaluate(initialState, dt, middleRight);

	vmath::vec3 dxdt = (1.0f / 6.0f) * (left.pos + 2.0f * (middleLeft.pos + middleRight.pos) + right.pos);
	vmath::vec3 dvdt = (1.0f / 6.0f) * (left.vel + 2.0f * (middleLeft.vel + middleRight.vel) + right.vel);

	pos += (dxdt * dt);
	vel += (dvdt * dt);
}

// Moves an item based on it's position, velocity, and timestep.
void Physica::ShipIntegration(vmath::vec3& pos, vmath::vec3& vel, float dt) const
{
	PosVel initialState;
	initialState.pos = pos;
	initialState.vel = vel;

	PosVel zero;
	zero.pos = vmath::vec3(0.0f, 0.0f, 0.0f);
	zero.vel = vmath::vec3(0.0f, 0.0f, 0.0f);

	PosVel left = RK4ShipStepEvaluate(initialState, 0.0f, zero);
	PosVel middleLeft = RK4ShipStepEvaluate(initialState, dt * 0.5f, left);
	PosVel middleRight = RK4ShipStepEvaluate(initialState, dt * 0.5f, middleLeft);
	PosVel right = RK4ShipStepEvaluate(initialState, dt, middleRight);

	vmath::vec3 dxdt = (1.0f / 6.0f) * (left.pos + 2.0f * (middleLeft.pos + middleRight.pos) + right.pos);
	vmath::vec3 dvdt = (1.0f / 6.0f) * (left.vel + 2.0f * (middleLeft.vel + middleRight.vel) + right.vel);

	pos += (dxdt * dt);
	vel += (dvdt * dt);
}

void Physica::Run()
{
    sf::Clock clock;
    while (isAlive)
    {
        if (!isPaused && initialized)
        {
			// Update the ship with user input.
			shipia->Update(didRotate, didTranslate);

            // Manage ship physics
            HandleShipMotion();

			// Manage the asteroids
			HandleAsteroidMotion();

			// Manage the sun
			stellaria->Update();

            // TODO manage game physics.
        }

        // The physics thread runs at a configurable delay, which we abide by here.
        sf::Time physicsUpdateTime = clock.restart();
        int sleepAmount = PhysicsConfig::PhysicsThreadDelay - physicsUpdateTime.asMilliseconds();
        if (sleepAmount > 0)
        {
            std::chrono::milliseconds sleepTime(sleepAmount);
            std::this_thread::sleep_for(sleepTime);
        }
    }
}

// Moves and rotates the asteroids according to sun gravity.
void Physica::HandleAsteroidMotion()
{
	for (unsigned int i = 0; i < asteroida->asteroids.positions.size(); i++)
	{
		// Note that we're storing custom data in the 4th spot, so simple addition fails.
		vmath::vec3 position = vmath::vec3(
			asteroida->asteroids.positions[i][0],
			asteroida->asteroids.positions[i][1],
			asteroida->asteroids.positions[i][2]);
		SunIntegration(position, asteroida->asteroids.velocities[i], PhysicsConfig::AsteroidTimestep);
		asteroida->asteroids.positions[i][0] = position[0];
		asteroida->asteroids.positions[i][1] = position[1];
		asteroida->asteroids.positions[i][2] = position[2];

		// Yaw, pitch, and roll for each asteroid.
		asteroida->asteroids.rotations[i] = vmath::quaternion::fromAxisAngle(asteroida->asteroids.eulerRotations[i][0],
			vmath::vec3(1, 0, 0)) * asteroida->asteroids.rotations[i];
		asteroida->asteroids.rotations[i] = vmath::quaternion::fromAxisAngle(asteroida->asteroids.eulerRotations[i][1],
			vmath::vec3(0, 1, 0)) * asteroida->asteroids.rotations[i];
		asteroida->asteroids.rotations[i] = vmath::quaternion::fromAxisAngle(asteroida->asteroids.eulerRotations[i][2],
			vmath::vec3(0, 0, 1)) * asteroida->asteroids.rotations[i];

		asteroida->asteroids.rotations[i].normalize();
	}
}

// Handles ship translation, rotation, and the dampeners.
void Physica::HandleShipMotion()
{
	// Note that I'm using the asteroid timestep, because else you cannot match orbital speeds with an asteroid!
	ShipIntegration(shipia->shipPosition, shipia->shipVelocity, PhysicsConfig::AsteroidTimestep);

    // Apply rotation (yaw, pitch, and then roll, in that order).
    // This is actually horrendously incorrect (physically speaking). However, it looks ok and works ok for small rotation speeds.
	shipia->shipOrientation = vmath::quaternion::fromAxisAngle(shipia->shipRotation[0], shipia->shipOrientation.upVector()) * shipia->shipOrientation;

    vmath::vec3 sidewaysVector = vmath::cross(shipia->shipOrientation.upVector(), shipia->shipOrientation.forwardVector());
	shipia->shipOrientation = vmath::quaternion::fromAxisAngle(shipia->shipRotation[1], sidewaysVector) * shipia->shipOrientation;

	shipia->shipOrientation = vmath::quaternion::fromAxisAngle(shipia->shipRotation[2], shipia->shipOrientation.forwardVector()) * shipia->shipOrientation;
	shipia->shipOrientation.normalize();

	if (!didTranslate && shipia->translationalDampener)
	{
		// Apply translational dampening
		// TODO use real dampening forces instead of just a multiplying force
		shipia->shipVelocity *= 0.95f;
	}
	
	if (!didRotate && shipia->rotationalDampener)
	{
		// Apply rotational dampening
		// TODO use real dampening forces instead of just a multiplying force
		shipia->shipRotation *= 0.95f;
	}
}

Physica::~Physica()
{
}
