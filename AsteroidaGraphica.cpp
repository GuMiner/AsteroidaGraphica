#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "AsteroidaGraphica.h"

#pragma comment(lib, "opengl32")

#ifndef _DEBUG
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/sfml-system")
#pragma comment(lib, "lib/sfml-window")
#else
#pragma comment(lib, "lib/glew32d.lib")
#pragma comment(lib, "lib/sfml-system-d")
#pragma comment(lib, "lib/sfml-window-d")
#endif

std::unique_ptr<Logger> AsteroidaGraphica::Log;

AsteroidaGraphica::AsteroidaGraphica()
{
}

Version::Status AsteroidaGraphica::Initialize()
{
    glewExperimental = TRUE;
    return Version::Status::OK;
}

Version::Status AsteroidaGraphica::Run()
{
    sf::Window window(sf::VideoMode(1366, 768), Version::NAME, sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        AsteroidaGraphica::Log->Log(Logger::ERR, "GLEW startup failure", err);
        return Version::Status::BAD_GLEW;
    }

    bool alive = true;
    while (alive)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                alive = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // Ensure our viewport always matches the screen size.
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.display();
    }


    return Version::Status::OK;
}

AsteroidaGraphica::~AsteroidaGraphica()
{
}

// Runs the main application.
int main(int argc, char* argv[])
{
    // Startup the log
    AsteroidaGraphica::Version = Version();
    AsteroidaGraphica::Log = std::unique_ptr<Logger>(new Logger("asteroid-graphica.log"));
    AsteroidaGraphica::Log->Log("Application Start!");

    Version::Status runStatus;
    std::unique_ptr<AsteroidaGraphica> asterioidaGraphica(new AsteroidaGraphica());
    
    // Run the application.
    runStatus = asterioidaGraphica->Initialize();
    if (runStatus == Version::Status::OK)
    {
        runStatus = asterioidaGraphica->Run();
    }
    else
    {
        AsteroidaGraphica::Log->Log(Logger::ERR, "Could not initialize the logger!");
    }

    // Wait before closing for display purposes.
    AsteroidaGraphica::Log->Log("Application End!");
    std::chrono::milliseconds sleepTime(1000);
    std::this_thread::sleep_for(sleepTime);

    // Log is auto-shutdown.
    return (int)runStatus;
}