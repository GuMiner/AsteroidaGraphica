#include <string>
#include <sstream>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "AsteroidaGraphica.h"
#include "Vertex.h"

#pragma comment(lib, "opengl32")

#ifndef _DEBUG
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/sfml-audio")
#pragma comment(lib, "lib/sfml-system")
#pragma comment(lib, "lib/sfml-window")
#else
#pragma comment(lib, "lib/glew32d.lib")
#pragma comment(lib, "lib/sfml-audio-d")
#pragma comment(lib, "lib/sfml-system-d")
#pragma comment(lib, "lib/sfml-window-d")
#endif

const char* Version::NAME;

AsteroidaGraphica::AsteroidaGraphica()
    : physicsManager(), physicaThread(&Physica::Run, &physicsManager), musicThread(&MusicManager::Run, &musicManager)
{
}

void AsteroidaGraphica::LogGraphicsSettings()
{
    std::stringstream graphicsSettings;
    graphicsSettings << "OpenGL vendor: " << glGetString(GL_VENDOR) << ", version " << glGetString(GL_VERSION) << ", renderer " << glGetString(GL_RENDERER);
    Logger::Log(graphicsSettings.str().c_str());
}

// Ensures our viewport letterboxes when it doesn't match the 16:9 aspect ratio.
void AsteroidaGraphica::UpdatePerspective(unsigned int width, unsigned int height)
{
    // Letterboxing is done at the top and bottom.
    float necessaryWidth = (float)height * Constants::ASPECT;
    if (necessaryWidth > width)
    {
        // Letterbox the top and the bottom of the screen so that the aspect ratio is met
        float effectiveHeight = (float)width / Constants::ASPECT;
        float heightDelta = ((float)height - effectiveHeight) / 2.0f;
        glViewport(0, (int)heightDelta, (GLsizei)width, (GLsizei)effectiveHeight);
    }
    else
    {
        // Letterbox the left and the right so that the aspect ratio is met.
        float widthDelta = ((float)width - necessaryWidth) / 2.0f;
        glViewport((int)widthDelta, 0, (GLsizei)necessaryWidth, (GLsizei)height);
    }
}

Constants::Status AsteroidaGraphica::Initialize()
{
    glewExperimental = TRUE;

    if (!configManager.ReadConfiguration())
    {
        return Constants::Status::BAD_CONFIG;
    }

    // TESTING CODE
    configManager.WriteConfiguration();

    physicaThread.launch();
    Logger::Log("Physica Thread Started!");

    musicThread.launch();
    Logger::Log("Music Thread Started!");

    return Constants::Status::OK;
}

// Loads first time graphics settings.
Constants::Status AsteroidaGraphica::LoadFirstTimeGraphics()
{
    // GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        Logger::LogErrorCode("GLEW startup failure", err);
        return Constants::Status::BAD_GLEW;
    }

    LogGraphicsSettings();

    // Basic OpenGL runtime settings
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_MULTISAMPLE);

    // Note that we do NOT cull faces, because that leads to incorrect see-through triangle behaviour.
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Perspective display
    perspectiveMatrix = vmath::perspective(Constants::FOV_Y, Constants::ASPECT, Constants::NEAR_PLANE, Constants::FAR_PLANE);

    // Shaders
    Logger::Log("Shader creation...");
    if (!shaderManager.CreateShaderProgram("render", &flatShaderProgram))
    {
        return Constants::Status::BAD_SHADERS;
    }

    mv_location = glGetUniformLocation(flatShaderProgram, "mv_matrix");
    proj_location = glGetUniformLocation(flatShaderProgram, "proj_matrix");

    Logger::Log("Shader creation done!");

    Constants::Status status = LoadAssets();
    if (status != Constants::Status::OK)
    {
        return status;
    }

    // TEST CODE TEST CODE
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Setup of vertex transfer (note we're using the "vertex" object in CodeGell)
    glGenBuffers(1, &pointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);

    // Send some random data to the GPU
    // This was a reload operation, changes were performed that must be flushed to the GPU.
    colorVertex *pVertices = new colorVertex[9];
    pVertices[0].Set(0, 0, 0, 1, 1, 1);
    pVertices[1].Set(0, 5, 0, 0, 1, 0);
    pVertices[2].Set(5, 0, 0, 1, 0, 0);

    pVertices[3].Set(0, 5, 0, 0, 1, 0);
    pVertices[4].Set(0, 0, 5, 0, 0, 1);
    pVertices[5].Set(0, 0, 0, 1, 1, 1);
    
    pVertices[6].Set(5, 0, 0, 1, 0, 0);
    pVertices[7].Set(0, 0, 5, 0, 0, 1);
    pVertices[8].Set(0, 0, 0, 1, 1, 1);

    vertexCount = 9;
    colorVertex::TransferToOpenGl(pVertices, vertexCount);
    
    delete[] pVertices;

    // HUD
    Logger::Log("HUD loading...");
    if (!shipHud.Initialize(&shaderManager, &fontManager, &imageManager))
    {
        return Constants::Status::BAD_HUD;
    }
    Logger::Log("HUD loading done!");

    return Constants::Status::OK;
}

// Loads up the in-game assets at the start of the game.
Constants::Status AsteroidaGraphica::LoadAssets()
{
    // Fonts
    Logger::Log("Font loading...");
    if (!fontManager.LoadFont(&shaderManager, "fonts/DejaVuSans.ttf"))
    {
        return Constants::Status::BAD_FONT;
    }

    Logger::Log("Font loading done!");

    // Sounds
    Logger::Log("Sound loading...");
    if (!soundManager.LoadSounds())
    {
        return Constants::Status::BAD_SOUND;
    }
    Logger::Log("Sound loading done!");

    // Music
    Logger::Log("Music loading...");
    if (!musicManager.LoadMusic())
    {
        return Constants::Status::BAD_MUSIC;
    }
    Logger::Log("Music loading done!");

    musicManager.Pause(); // TODO temp code (annoying when developing)

    // Physica
    Logger::Log("Physica loading...");
    physicsManager.Initialize(&soundManager);
    Logger::Log("Physica loading done!");

    return Constants::Status::OK;
}

Constants::Status AsteroidaGraphica::Run()
{
    // 24 depth bits, 8 stencil bits, 8x AA, major version 3.
    Logger::Log("Graphics Initializing...");
    sf::ContextSettings contextSettings = sf::ContextSettings(24, 8, 8, 3, 0, 0);

    sf::Uint32 style = ConfigManager::IsFullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close;
    sf::Window window(sf::VideoMode(ConfigManager::ScreenWidth, ConfigManager::ScreenHeight), Version::NAME, style, contextSettings);
    Constants::Status firstTimeSetup = LoadFirstTimeGraphics();
    if (firstTimeSetup != Constants::Status::OK)
    {
        return firstTimeSetup;
    }

    UpdatePerspective(window.getSize().x, window.getSize().y);
    Logger::Log("Graphics Initialized!");
    
    sf::Clock clock;
    sf::Time clockStartTime;
    bool alive = true;
    bool paused = false;
    while (alive)
    {
        clockStartTime = clock.getElapsedTime();

        // Handle all events.
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                alive = false;
            }
            else if (event.type == sf::Event::LostFocus)
            {
                paused = true;
                physicsManager.Pause();
                musicManager.Pause();
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                paused = false;
                physicsManager.Resume();
                musicManager.Resume();
            }
            else if (event.type == sf::Event::Resized)
            {
                UpdatePerspective(event.size.width, event.size.height);
            }
        }

        // Render, only if non-paused.
        if (!paused)
        {
            // Clear the screen, firstoff
            const GLfloat color[] = { 0, 0, 0, 1 };
            const GLfloat one = 1.0f;
            glClearBufferfv(GL_COLOR, 0, color);
            glClearBufferfv(GL_DEPTH, 0, &one);
            
            // TEST CODE DRAWING
            glUseProgram(flatShaderProgram);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);

            // *physicsManager.shipOrientation;
            lookAtMatrix = vmath::lookat(physicsManager.shipPosition, vmath::vec3(0, 0, 0), vmath::vec3(0, 1, 0));
            vmath::mat4 quatMatrix = physicsManager.shipOrientation.asMatrix() * vmath::translate(-physicsManager.shipPosition);
            vmath::mat4 result = perspectiveMatrix * quatMatrix; // lookAtMatrix;
            glUniformMatrix4fv(proj_location, 1, GL_FALSE, result);

            // Let's add a 5x5x5 grid of these things to help with debugging.
            float separation = 7;
            int scale = 4;
            for (int i = 0; i < scale; i++)
            {
                for (int j = 0; j < scale; j++)
                {
                    for (int k = 0; k < scale; k++)
                    {
                        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(i*separation, j*separation, k*separation));
                        glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
                        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
                    }
                }
            }

            // Draws our HUD
            shipHud.UpdateCompassRotations(physicsManager.shipOrientation.asEulerAngles());
            shipHud.UpdateShipPositition(physicsManager.shipPosition);
            shipHud.RenderHud(perspectiveMatrix, clock);

            window.display();
        }

        sf::Int64 sleepDelay = (1000000 / Constants::MAX_FRAMERATE) - clock.getElapsedTime().asMicroseconds() - clockStartTime.asMicroseconds();
        if (sleepDelay > 0)
        {
            std::chrono::microseconds sleepTime(sleepDelay);
            std::this_thread::sleep_for(sleepTime);
        }
    }


    return Constants::Status::OK;
}

void AsteroidaGraphica::Deinitialize()
{
    // Application shutdown.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &pointBuffer);

    Logger::Log("Music Thread Stopping...");
    musicManager.Stop();

    Logger::Log("Physica Thread Stopping...");
    physicsManager.Stop();
    physicaThread.wait();
    Logger::Log("Physica Thread Stopped.");

    musicThread.wait();
    Logger::Log("Music Thread Stopped.");
}

// Runs the main application.
int main(int argc, char* argv[])
{
    // Startup 'static' stuff
    AsteroidaGraphica::Version = Version();
    AsteroidaGraphica::Constant = Constants();

    Logger::Setup();
    
    std::stringstream startupDetails;
    startupDetails << Version::NAME << " " << Version::MAJOR_VERSION << "." << Version::MINOR_VERSION << std::endl;
    Logger::Log(startupDetails.str().c_str());

    Constants::Status runStatus;
    std::unique_ptr<AsteroidaGraphica> asterioidaGraphica(new AsteroidaGraphica());
    
    // Run the application.
    runStatus = asterioidaGraphica->Initialize();
    if (runStatus == Constants::Status::OK)
    {
        runStatus = asterioidaGraphica->Run();
        asterioidaGraphica->Deinitialize();
    }
    else
    {
        Logger::LogError("Could not initialize the main program!");
    }

    // Wait before closing for display purposes.
    Logger::Log("Application End!");
    Logger::Shutdown();
    std::chrono::milliseconds sleepTime(1000);
    std::this_thread::sleep_for(sleepTime);

    // Log is auto-shutdown.
    return (int)runStatus;
}