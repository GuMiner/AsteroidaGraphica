#include <sstream>
#include <thread>
#include <SFML\System.hpp>
#include "Logger.h"
#include "MusicManager.h"

MusicManager::MusicManager()
{
    isAlive = true;
    isPaused = false;
    initialized = false;

    currentPitch = 1.0f;
    pitchStep = 0.0033f;

    stepsForMoodChange = 4; // 2 seconds (@ 30 fps physics code).
    heightenSteps = 0;
    lowerSteps = 0;
}

// Loads up an individual song.
bool MusicManager::LoadSong(Area area, const char* songName)
{
    sf::Music *music = new sf::Music();
    if (!music->openFromFile(songName))
    {
        std::stringstream errStream;
        errStream << "Could not load the " << songName << " song.";
        Logger::LogError(errStream.str().c_str());
        return false;
    }
    
    musicData[area].push_back(music);
    return true;
}

// Loads up the current music data
bool MusicManager::LoadMusic()
{
    // TODO eventually this should be loaded just by reading the directory.
    if (!LoadSong(OPEN_SPACE, "music/openSpace/deadlock.ogg")) // OPEN SPACE
    {
        return false;
    }
    else if (!LoadSong(OPEN_SPACE, "music/openSpace/greyscale.ogg"))
    {
        return false;
    }
    else if (!LoadSong(ASTEROIDS, "music/asteroids/wakeup.ogg")) // ASTEROIdS
    {
        return false;
    }
    else if (!LoadSong(ASTEROIDS, "music/asteroids/cosmos.ogg"))
    {
        return false;
    }

    currentArea = OPEN_SPACE;
    currentSong = 0;

    musicData[currentArea][currentSong]->play();
    initialized = true;
    return true;
}

// Stops the current audio with audio for the new area.
void MusicManager::SwitchArea(Area area)
{
    if (currentArea == area)
    {
        // shortcut out.
        return;
    }

    // Switch song banks if needed.
    areaSwitcherMutex.lock();
    musicData[currentArea][currentSong]->stop();

    currentArea = area;
    currentSong = 0;
    musicData[currentArea][currentSong]->play();
    areaSwitcherMutex.unlock();
}

// Adds heighten steps so that the music pitch is increased in speed.
void MusicManager::HeightenMood()
{
    heightenSteps += stepsForMoodChange;
}

// Adds lower steps so that the music pitch is decreased in speed.
void MusicManager::LowerMood()
{
    lowerSteps += stepsForMoodChange;
}

// Resets the mood of the song to the default.
void MusicManager::ResetMood()
{
    heightenSteps = 0;
    lowerSteps = 0;
    currentPitch = 1.0f;
}

// Runs the music thread, advancing tracks and updating the pitch for special effects.
void MusicManager::Run()
{
    sf::Clock clock;
    bool wasPaused = false;
    while (isAlive)
    {
        if (initialized)
        {
            if (isPaused && !wasPaused)
            {
                // We entered a pause phase, so pause the currently-active music.
                musicData[currentArea][currentSong]->pause();
                wasPaused = true;
            }
            else if (!isPaused && wasPaused)
            {
                // We exited a pause phase, so unpause the currently-active music.
                musicData[currentArea][currentSong]->play();
                wasPaused = false;
            }
        }

        // Handle music and mood changes.
        if (!isPaused && initialized)
        {
            // Apply any mood changes.
            if (heightenSteps > 0 && lowerSteps > 0)
            {
                // Mood changes cancel out.
                --heightenSteps;
                --lowerSteps;
            }
            else if (heightenSteps > 0)
            {
                --heightenSteps;
                currentPitch += pitchStep;
            }
            else if (lowerSteps > 0)
            {
                --lowerSteps;
                currentPitch -= pitchStep;
            }

            areaSwitcherMutex.lock();

            // Switch to the next track (or reset to the beginning tracks), if we've stopped playing our current song.
            if (musicData[currentArea][currentSong]->getStatus() == sf::SoundSource::Status::Stopped)
            {
                ++currentSong;
                if (currentSong > (int)musicData[currentArea].size())
                {
                    currentSong = 0;
                }

                musicData[currentArea][currentSong]->play();
            }

            // Always update the pitch so the mood stays consistent.
            musicData[currentArea][currentSong]->setPitch(currentPitch);
            areaSwitcherMutex.unlock();
        }

        // The music thread, like the physics thread, updates at a constant 30 fps.
        sf::Time musicUpdateTime = clock.restart();
        int sleepTime = 33 - musicUpdateTime.asMilliseconds();
        if (sleepTime > 0)
        {
            std::chrono::milliseconds sleepTime(33);
            std::this_thread::sleep_for(sleepTime);
        }
    }
}

MusicManager::~MusicManager()
{
    // Delete the music tracks we loaded
    for (std::map<Area, std::vector<sf::Music*>>::iterator iterator = musicData.begin(); iterator != musicData.end(); iterator++)
    {
        for (unsigned int i = 0; i < iterator->second.size(); i++)
        {
            delete iterator->second[i];
        }
    }
}
