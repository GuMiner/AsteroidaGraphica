#include <SFML\System.hpp>
#include <thread>
#include "Physica.h"


Physica::Physica()
{
    isAlive = true;
}

void Physica::Run()
{
    sf::Clock clock;
    while (isAlive)
    {
        // TODO manage asteroid physics.

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
