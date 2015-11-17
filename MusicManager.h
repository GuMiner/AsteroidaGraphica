#pragma once
#include <map>
#include <vector>
#include <SFML\Audio.hpp>

class MusicManager
{
    // Used for music categories. Currently unused except for categories.
    enum Area
    {
        OPEN_SPACE,
        ASTEROIDS
    };

    std::map<Area, std::vector<sf::Music*>> musicData;
    int currentSong;
    sf::Mutex areaSwitcherMutex;

    // Current sound parameters
    Area currentArea;
    float currentPitch;
    
    float pitchStep;
    int stepsForMoodChange;
    int heightenSteps;
    int lowerSteps;

    bool isAlive;
    bool isPaused;
    bool initialized;

    bool LoadSong(Area area, const char* songName);

public:
    MusicManager();
    bool LoadMusic();
    void Run();
    
    void SwitchArea(Area area);
    void HeightenMood();
    void LowerMood();
    void ResetMood();

    void Pause()
    {
        isPaused = true;
    }

    void Resume()
    {
        isPaused = false;
    }

    void Stop()
    {
        isAlive = false;
    }

    ~MusicManager();
};

