#include <thread>
#include "ConfigManager.h"
#include "Physica.h"

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

	float distance = vmath::distance(pos, vmath::vec3(0, 0, 0));
	float constants = -ConfigManager::SolarMass * ConfigManager::GravitationalConstant / distance;

	return constants * vmath::normalize(pos);
}

// Performs RK4 step evaluation within the context of solar masse.
PosVel Physica::RK4SunStepEvaluate(const PosVel& initialState, float dt, const PosVel& derivative) const
{
	PosVel result;
	result.pos = initialState.vel + derivative.vel * dt;
	result.vel = SunAcceleration(initialState.pos + derivative.pos * dt);
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
	vmath::vec3 dvdt = (1.0f / 6.0f) * (right.vel + 2.0f * (middleLeft.vel + middleRight.vel) + right.vel);

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
			bool didRotate, didTranslate;
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
        int sleepAmount = ConfigManager::PhysicsThreadDelay - physicsUpdateTime.asMilliseconds();
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
	asteroida->updateMutex.lock();
	for (unsigned int i = 0; i < asteroida->positions.size(); i++)
	{
		// Note that we're storing custom data in the 4th spot, so simple addition fails.
		vmath::vec3 position = vmath::vec3(asteroida->positions[i][0], asteroida->positions[i][1], asteroida->positions[i][2]);
		SunIntegration(position, asteroida->velocities[i], ConfigManager::AsteroidTimestep);
		asteroida->positions[i][0] = position[0];
		asteroida->positions[i][1] = position[1];
		asteroida->positions[i][2] = position[2];

		// Yaw, pitch, and roll for each asteroid.
		asteroida->rotations[i] = vmath::quaternion::fromAxisAngle(asteroida->eulerRotations[i][0], 
			vmath::vec3(1, 0, 0)) * asteroida->rotations[i];
		asteroida->rotations[i] = vmath::quaternion::fromAxisAngle(asteroida->eulerRotations[i][1],
			vmath::vec3(0, 1, 0)) * asteroida->rotations[i];
		asteroida->rotations[i] = vmath::quaternion::fromAxisAngle(asteroida->eulerRotations[i][2],
			vmath::vec3(0, 0, 1)) * asteroida->rotations[i];

		asteroida->rotations[i].normalize();
	}

	asteroida->updateMutex.unlock();
	asteroida->updatedAsteroidPositionAndRotation = true;
}

void Physica::HandleShipMotion()
{
    // Apply forces.
    // TODO use an actual physical algorithm (RK4) instead of Euler integration.
    // F = ma
    vmath::vec3 shipAccel = shipia->shipForce / shipia->GetShipMass();
    // v = a*deltaT, x = v*deltaT
    // deltaT =  33 ms
    shipia->shipVelocity += shipAccel * (33.0f / 1000.0f);
	shipia->shipPosition += shipia->shipVelocity * (33.0f / 1000.0f);

    // Apply rotation (yaw, pitch, and then roll, in that order).
    // This is actually horrendously incorrect (physically speaking). However, it looks ok and works ok for small rotation speeds.
	shipia->shipOrientation = vmath::quaternion::fromAxisAngle(shipia->shipRotation[0], shipia->shipOrientation.upVector()) * shipia->shipOrientation;

    vmath::vec3 sidewaysVector = vmath::cross(shipia->shipOrientation.upVector(), shipia->shipOrientation.forwardVector());
	shipia->shipOrientation = vmath::quaternion::fromAxisAngle(shipia->shipRotation[1], sidewaysVector) * shipia->shipOrientation;

	shipia->shipOrientation = vmath::quaternion::fromAxisAngle(shipia->shipRotation[2], shipia->shipOrientation.forwardVector()) * shipia->shipOrientation;
	shipia->shipOrientation.normalize();
}

Physica::~Physica()
{
}
