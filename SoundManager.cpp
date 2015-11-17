#include "AsteroidaGraphica.h"
#include "SoundManager.h"



SoundManager::SoundManager()
{
    // Set where the sound listener is pointing to.
    sf::Listener::setPosition(0.0f, 0.0f, 0.0f);
    sf::Listener::setDirection(0.0f, 0.0f, 1.0f);
    sf::Listener::setUpVector(0.0f, 1.0f, 0.0f);
}

// Loads up all the sounds in AsteroidaGraphica.
bool SoundManager::LoadSounds()
{
    soundBuffers[THRUST] = new sf::SoundBuffer();
    if (!soundBuffers[THRUST]->loadFromFile("sounds/thrust.ogg"))
    {
        AsteroidaGraphica::Log->Log(Logger::LogType::ERR, "Could not load the thrust sound!");
        return false;
    }

    soundBuffers[BUTTON_TOGGLE] = new sf::SoundBuffer();
    if (!soundBuffers[BUTTON_TOGGLE]->loadFromFile("sounds/button_toggle.ogg"))
    {
        AsteroidaGraphica::Log->Log(Logger::LogType::ERR, "Could not load the button toggle sound!");
        return false;
    }

    soundBuffers[ROTATE] = new sf::SoundBuffer();
    if (!soundBuffers[ROTATE]->loadFromFile("sounds/rotate.ogg"))
    {
        AsteroidaGraphica::Log->Log(Logger::LogType::ERR, "Could not load the rotate sound!");
        return false;
    }

    return true;
}

// Cleans the queue by capping the max number of active sounds.
void SoundManager::CleanQueue()
{
    if ((int)activeSounds.size() > QUEUE_SOUND_CAP)
    {
        activeSounds.front()->stop();
        delete activeSounds.front();
        activeSounds.pop();
    }
}

// Plays the specified sound at position (absolute), with minimum hearable distance (playback.x), attenuation (0 to 100, playback.y), and volume (0 to 100, playback.z)
// Because we *never* move the listener position, this means that +x = left of listener, +y = above listener, and +z = forward of listener.
void SoundManager::PlaySound(Sound soundType, sf::Vector3f soundPosition, sf::Vector3f soundPlaybackDetails)
{
    CleanQueue();

    sf::Sound *sound = new sf::Sound();
    sound->setBuffer(*soundBuffers[soundType]);

    sound->setPosition(soundPosition);
    sound->setMinDistance(soundPlaybackDetails.x);
    sound->setAttenuation(soundPlaybackDetails.y);
    sound->setVolume(soundPlaybackDetails.z);

    sound->play();
    activeSounds.emplace(sound);
}

SoundManager::~SoundManager()
{
    while (activeSounds.size() != 0)
    {
        activeSounds.front()->stop();
        delete activeSounds.front();
        activeSounds.pop();
    }

    // Delete any sound buffers we created.
    for (std::map<Sound, sf::SoundBuffer*>::iterator iterator = soundBuffers.begin(); iterator != soundBuffers.end(); iterator++)
    {
        delete iterator->second;
        iterator->second = nullptr;
    }
}
