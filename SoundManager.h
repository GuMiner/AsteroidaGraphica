#pragma once
#include <map>
#include <queue>
#include <SFML\Audio.hpp>

class SoundManager
{
public:
    enum Sound
    {
        THRUST,
        ROTATE,
        BUTTON_TOGGLE,
    };

private:
    std::map<Sound, sf::SoundBuffer*> soundBuffers;
    std::queue<sf::Sound*> activeSounds;
    const int QUEUE_SOUND_CAP = 20;

    void CleanQueue();
    
public:
    SoundManager();
    bool LoadSounds();
    void PlaySound(Sound soundType, sf::Vector3f soundPosition, sf::Vector3f soundPlaybackDetails);
    ~SoundManager();
};

