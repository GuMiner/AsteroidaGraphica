#include <SFML\System.hpp>
#include <thread>
#include "Physica.h"


Physica::Physica()
    : shipOrientation(vmath::radians(0), vmath::vec3(-0.577, -0.577, -0.577))
{
    isAlive = true;

    shipPosition = vmath::vec3(8, 8, 8);

    shipVelocity = vmath::vec3(0, 0, 0);
    shipForce = vmath::vec3(0, 0, 0);
    shipMass = 1000; // 1 ton (kg)
}

void Physica::Run()
{
    sf::Clock clock;
    while (isAlive)
    {
        if (!isPaused)
        {
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

void Physica::Stop()
{
    isAlive = false;
}

Physica::~Physica()
{
}
