#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <thread>
#include "Physica.h"


Physica::Physica()
{
    isAlive = true;

    shipPosition = vmath::vec3(0, 0, 8);
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

void Physica::Initialize(SoundManager *soundManager)
{
    this->soundManager = soundManager;
}

void Physica::Run()
{
    sf::Clock clock;
    while (isAlive)
    {
        if (!isPaused)
        {
            // Manage ship physics
            shipForce = vmath::vec3(0.0f, 0.0f, 0.0f);
            HandleShipControls();
            HandleShipMotion();

            // TODO manage asteroid and game physics.
        }

        // The physics thread runs at a constant 30 updates-per-second. We attempt to ensure we don't wait too long here.
        sf::Time physicsUpdateTime = clock.restart();
        int sleepTime = 33 - physicsUpdateTime.asMilliseconds();
        if (sleepTime > 0)
        {
            std::chrono::milliseconds sleepTime(33);
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, -3, -5), sf::Vector3f(8, 0, thrustVolume));
        Thrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, 3, 5), sf::Vector3f(8, 0, thrustVolume));
        Thrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        // Thrust left and play a right thrust sound.
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(-5, 0, 0), sf::Vector3f(8, 0, thrustVolume));
        SideThrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        // Thrust right, sound on left
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(5, 0, 0), sf::Vector3f(8, 0, thrustVolume));
        SideThrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, -5, 0), sf::Vector3f(8, 0, thrustVolume));
        VerticalThrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, 5, 0), sf::Vector3f(8, 0, thrustVolume));
        VerticalThrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        RotateHorizontal(true);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        RotateHorizontal(false);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        RotateVertical(true);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        RotateVertical(false);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        BarrelRoll(true);
        didRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !rotDampToggled)
    {
        soundManager->PlaySound(SoundManager::BUTTON_TOGGLE, sf::Vector3f(0, 0, 0), sf::Vector3f(5, 0, 100));

        rotationalDampener = !rotationalDampener;
        rotDampToggled = true;
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        rotDampToggled = false;
    }

    // Toggles on/off the translational dampener
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !tranDampToggled)
    {
        soundManager->PlaySound(SoundManager::BUTTON_TOGGLE, sf::Vector3f(0, 0, 0), sf::Vector3f(5, 0, 100));

        translationalDampener = !translationalDampener;
        tranDampToggled = true;
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        tranDampToggled = false;
    }
}

void Physica::Stop()
{
    isAlive = false;
}

Physica::~Physica()
{
}
