#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <thread>
#include "Physica.h"


Physica::Physica()
{
    isAlive = true;

    shipPosition = vmath::vec3(0, 0, 8);
    shipOrientation = vmath::quaternion(0, 0, -1, 0);
    
    shipVelocity = vmath::vec3(0, 0, 0);
    shipForce = vmath::vec3(0, 0, 0);
    shipMass = 1000; // 1 ton (kg)
}

void Physica::Thrust(bool forwards)
{
    // TODO tie into a physics algorithm to integrate the accel and all that fancy stuff
    float speed = 0.07f;
    if (forwards)
    {
        shipPosition += shipOrientation.forwardVector() * speed;
    }
    else
    {
        shipPosition -= shipOrientation.forwardVector() * speed;
    }
}

void Physica::SideThrust(bool left)
{
    float speed = 0.07f;
    vmath::vec3 sidewaysVector = vmath::cross(shipOrientation.upVector(), shipOrientation.forwardVector());
    if (left)
    {
        shipPosition += speed*sidewaysVector;
    }
    else
    {
        shipPosition -= speed*sidewaysVector;
    }
}

void Physica::VerticalThrust(bool up)
{
    float speed = 0.07f;
    if (up)
    {
        shipPosition += shipOrientation.upVector() * speed;
    }
    else
    {
        shipPosition -= shipOrientation.upVector() * speed;
    }
}

void Physica::RotateHorizontal(bool left)
{
    float speed = vmath::radians(1.0f);
    shipOrientation = vmath::quaternion::fromAxisAngle(left ? -speed : speed, shipOrientation.upVector()) * shipOrientation;
    shipOrientation.normalize();
}

void Physica::RotateVertical(bool up)
{
    float speed = vmath::radians(1.0f);
    vmath::vec3 sidewaysVector = vmath::cross(shipOrientation.upVector(), shipOrientation.forwardVector());
    shipOrientation = vmath::quaternion::fromAxisAngle(up ? -speed : speed, sidewaysVector) * shipOrientation;
    shipOrientation.normalize();
}

void Physica::BarrelRoll(bool clockwise)
{
    float speed = vmath::radians(1.0f);
    shipOrientation = vmath::quaternion::fromAxisAngle(clockwise ? speed : -speed, shipOrientation.forwardVector()) * shipOrientation;
    shipOrientation.normalize();
}

void Physica::Run()
{
    sf::Clock clock;
    while (isAlive)
    {
        if (!isPaused)
        {
            // Manage ship physics
            HandleStandardMotion();

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

// Handles standard ship motion key commands.
void Physica::HandleStandardMotion()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        Thrust(true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        Thrust(false);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        SideThrust(true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        SideThrust(false);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        VerticalThrust(true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        VerticalThrust(false);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        RotateHorizontal(true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        RotateHorizontal(false);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        RotateVertical(true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        RotateVertical(false);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        BarrelRoll(true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        BarrelRoll(false);
    }
}

void Physica::Stop()
{
    isAlive = false;
}

Physica::~Physica()
{
}
