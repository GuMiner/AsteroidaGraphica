#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <thread>
#include "ConfigManager.h"
#include "Physica.h"

Physica::Physica()
{
    isAlive = true;
    isPaused = false;
    initialized = false;

    shipPosition = vmath::vec3(0, 0, 80);
    shipOrientation = vmath::quaternion(0, 0, 0, 1);
    
    shipVelocity = vmath::vec3(0, 0, 0);
    shipForce = vmath::vec3(0, 0, 0);
    shipRotation = vmath::vec3(0, 0, 0); // Radians / tick (@ 30 ticks/sec)
    shipMass = 1000; // 1 ton (kg)

    shipThrustSpeed = 190.0f;
    shipSideThrustSpeed = 190.0f;
    shipVerticalThrustSpeed = 190.0f;
    transDampenerThrustSpeed = 5.0f;

    shipHorizRotSpeed = 0.0015f; 
    shipVertRotSpeed = 0.0015f;
    shipBarrelRollSpeed = 0.0015f;
    rotDampenerSpeed = 0.00025f;

    rotationalDampener = false;
    translationalDampener = false;

    rotDampToggled = false;
    tranDampToggled = false;

    didTranslate = false;
    didRotate = false;
}

void Physica::Thrust(bool forwards)
{
    shipForce += (forwards ? shipThrustSpeed : -shipThrustSpeed) * shipOrientation.forwardVector();
}

void Physica::SideThrust(bool left)
{
    vmath::vec3 sidewaysVector = vmath::cross(shipOrientation.upVector(), shipOrientation.forwardVector());
    shipForce += (left ? -shipSideThrustSpeed : shipSideThrustSpeed) * sidewaysVector;
}

void Physica::VerticalThrust(bool up)
{
    shipForce += (up ? -shipVerticalThrustSpeed : shipVerticalThrustSpeed) * shipOrientation.upVector();
}

void Physica::RotateHorizontal(bool left)
{
    shipRotation[0] += left ? -shipHorizRotSpeed : shipHorizRotSpeed;
    
}

void Physica::RotateVertical(bool up)
{
    shipRotation[1] += up ? -shipVertRotSpeed : shipVertRotSpeed;
}

void Physica::BarrelRoll(bool clockwise)
{
    shipRotation[2] += clockwise ? shipBarrelRollSpeed : -shipBarrelRollSpeed;
}

void Physica::Initialize(SoundManager *soundManager, Asteroida *asteroida, Stellaria *stellaria)
{
    this->soundManager = soundManager;
	this->asteroida = asteroida;
	this->stellaria = stellaria;
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
            // Manage ship physics
            shipForce = vmath::vec3(0.0f, 0.0f, 0.0f);
            HandleShipControls();
            HandleShipMotion();

			// Manage the asteroids
			asteroida->updateMutex.lock();
			for (unsigned int i = 0; i < asteroida->positions.size(); i++)
			{
				// Note that we're storing custom data in the 4th spot, so simple addition fails.
				vmath::vec3 position = vmath::vec3(asteroida->positions[i][0], asteroida->positions[i][1], asteroida->positions[i][2]);
				SunIntegration(position, asteroida->velocities[i], ConfigManager::AsteroidTimestep);	
				asteroida->positions[i][0] = position[0];
				asteroida->positions[i][1] = position[1];
				asteroida->positions[i][2] = position[2];
			}

			asteroida->updateMutex.unlock();
			asteroida->updatedAsteroidPosition = true;

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

void Physica::HandleShipMotion()
{
    // Apply forces.
    // TODO use an actual physical algorithm (RK4) instead of Euler integration.
    // F = ma
    vmath::vec3 shipAccel = shipForce / shipMass;
    // v = a*deltaT, x = v*deltaT
    // deltaT =  33 ms
    shipVelocity += shipAccel * (33.0f / 1000.0f);
    shipPosition += shipVelocity * (33.0f / 1000.0f);

    // Apply rotation (yaw, pitch, and then roll, in that order).
    // This is actually horrendously incorrect (physically speaking). However, it looks ok and works ok for small rotation speeds.
    shipOrientation = vmath::quaternion::fromAxisAngle(shipRotation[0], shipOrientation.upVector()) * shipOrientation;

    vmath::vec3 sidewaysVector = vmath::cross(shipOrientation.upVector(), shipOrientation.forwardVector());
    shipOrientation = vmath::quaternion::fromAxisAngle(shipRotation[1], sidewaysVector) * shipOrientation;

    shipOrientation = vmath::quaternion::fromAxisAngle(shipRotation[2], shipOrientation.forwardVector()) * shipOrientation;
    shipOrientation.normalize();

    if (!didTranslate && translationalDampener)
    {
        // Apply translational dampening
        // TODO use real dampening forces instead of just a multiplying force
        shipVelocity *= 0.95f;
        float shipVelLength = sqrtf(shipVelocity[0] * shipVelocity[0] + shipVelocity[1] * shipVelocity[1] + shipVelocity[2] * shipVelocity[2]);
        if (shipVelLength > 3)
        {
            sf::Vector3f reverseVector = sf::Vector3f(-shipVelocity[0] / shipVelLength, -shipVelocity[1] / shipVelLength, -shipVelocity[2] / shipVelLength);
            
            // We need to apply force to dampen, so we play a sound of us thrusting in the right direction.
            soundManager->PlaySound(SoundManager::THRUST, reverseVector, sf::Vector3f(3, 0, 10));
        }
    }

    if (!didRotate && rotationalDampener)
    {
        // Apply rotational dampening
        // TODO use real dampening forces instead of just a multiplying force
        shipRotation *= 0.95f;
        if (shipRotation[0] * shipRotation[0] + shipRotation[1] * shipRotation[1] + shipRotation[2] * shipRotation[2] > 9)
        {
            // We need to apply force to dampen, so we play a sound!
            soundManager->PlaySound(SoundManager::ROTATE, sf::Vector3f(0, 0, 0), sf::Vector3f(8, 0, 10));
        }
    }
}

// Handles standard ship motion key commands.
void Physica::HandleShipControls()
{
    didRotate = false;
    didTranslate = false;

    float thrustVolume = 40;
    if (sf::Keyboard::isKeyPressed(ConfigManager::ThrustForwardsKey))
    {
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, -3, -5), sf::Vector3f(8, 0, thrustVolume));
        Thrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::ThrustReverseKey))
    {
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, 3, 5), sf::Vector3f(8, 0, thrustVolume));
        Thrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::ThrustLeftKey))
    {
        // Thrust left and play a right thrust sound.
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(-5, 0, 0), sf::Vector3f(8, 0, thrustVolume));
        SideThrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::ThrustRightKey))
    {
        // Thrust right, sound on left
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(5, 0, 0), sf::Vector3f(8, 0, thrustVolume));
        SideThrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::ThrustUpKey))
    {
        // Thrust up, sound below.
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, -5, 0), sf::Vector3f(8, 0, thrustVolume));
        VerticalThrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::ThrustDownKey))
    {
        // Thrust down, sound above.
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, 5, 0), sf::Vector3f(8, 0, thrustVolume));
        VerticalThrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::RotateLeftKey))
    {
        RotateHorizontal(true);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::RotateRightKey))
    {
        RotateHorizontal(false);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::RotateUpKey))
    {
        RotateVertical(true);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::RotateDownKey))
    {
        RotateVertical(false);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::RotateCWKey))
    {
        BarrelRoll(true);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(ConfigManager::RotateCCWKey))
    {
        BarrelRoll(false);
        didRotate = true;
    }

    // If we manually rotated, play the rotation sound
    if (didRotate)
    {
        soundManager->PlaySound(SoundManager::ROTATE, sf::Vector3f(0, 0, 0), sf::Vector3f(8, 0, 20));
    }

    // Toggles on/off the rotational dampener
    if (sf::Keyboard::isKeyPressed(ConfigManager::ToggleRotationDampeningKey) && !rotDampToggled)
    {
        soundManager->PlaySound(SoundManager::BUTTON_TOGGLE, sf::Vector3f(0, 0, 0), sf::Vector3f(5, 0, 100));

        rotationalDampener = !rotationalDampener;
        rotDampToggled = true;
    }
    else if (!sf::Keyboard::isKeyPressed(ConfigManager::ToggleRotationDampeningKey))
    {
        rotDampToggled = false;
    }

    // Toggles on/off the translational dampener
    if (sf::Keyboard::isKeyPressed(ConfigManager::ToggleTranslationDampeningKey) && !tranDampToggled)
    {
        soundManager->PlaySound(SoundManager::BUTTON_TOGGLE, sf::Vector3f(0, 0, 0), sf::Vector3f(5, 0, 100));

        translationalDampener = !translationalDampener;
        tranDampToggled = true;
    }
    else if (!sf::Keyboard::isKeyPressed(ConfigManager::ToggleTranslationDampeningKey))
    {
        tranDampToggled = false;
    }
}

Physica::~Physica()
{
}
