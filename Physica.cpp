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

    shipThrustSpeed = 10.0f;
    shipSideThrustSpeed = 10.0f;
    shipVerticalThrustSpeed = 10.0f;
    transDampenerThrustSpeed = 5.0f;

    shipHorizRotSpeed = 0.5f; 
    shipVertRotSpeed = 0.5f;
    shipBarrelRollSpeed = 0.5f;
    rotDampenerSpeed = 0.25f;

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
    }

    if (!didRotate && rotationalDampener)
    {
        // Apply rotational dampening
    }
}

// Handles standard ship motion key commands.
void Physica::HandleShipControls()
{
    didRotate = false;
    didTranslate = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        Thrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        Thrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        SideThrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        SideThrust(false);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        VerticalThrust(true);
        didTranslate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !rotDampToggled)
    {
        rotationalDampener = !rotationalDampener;
        rotDampToggled = true;
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        rotDampToggled = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !tranDampToggled)
    {
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
