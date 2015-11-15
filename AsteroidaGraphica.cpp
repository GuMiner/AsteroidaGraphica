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
#pragma comment(lib, "lib/sfml-system")
#pragma comment(lib, "lib/sfml-window")
#else
#pragma comment(lib, "lib/glew32d.lib")
#pragma comment(lib, "lib/sfml-system-d")
#pragma comment(lib, "lib/sfml-window-d")
#endif

std::unique_ptr<Logger> AsteroidaGraphica::Log;

AsteroidaGraphica::AsteroidaGraphica()
    : physicsManager(), physicaThread(&Physica::Run, &physicsManager)
{
}

void AsteroidaGraphica::LogGraphicsSettings()
{
    std::stringstream graphicsSettings;
    graphicsSettings << "OpenGL vendor: " << glGetString(GL_VENDOR) << ", version " << glGetString(GL_VERSION) << ", renderer " << glGetString(GL_RENDERER);
    AsteroidaGraphica::Log->Log(graphicsSettings.str().c_str());
}

// Ensures our viewport letterboxes when it doesn't match the 16:9 aspect ratio.
void AsteroidaGraphica::UpdatePerspective(unsigned int width, unsigned int height)
{
    // Letterboxing is done at the top and bottom.
    float necessaryWidth = (float)height * Version::ASPECT;
    if (necessaryWidth > width)
    {
        // Letterbox the top and the bottom of the screen so that the aspect ratio is met
        float effectiveHeight = (float)width / Version::ASPECT;
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

Version::Status AsteroidaGraphica::Initialize()
{
    glewExperimental = TRUE;
    physicaThread.launch();
    AsteroidaGraphica::Log->Log("Physica Thread Started!");

    return Version::Status::OK;
}

// Loads first time graphics settings.
Version::Status AsteroidaGraphica::LoadFirstTimeGraphics()
{
    // GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        AsteroidaGraphica::Log->Log(Logger::ERR, "GLEW startup failure", err);
        return Version::Status::BAD_GLEW;
    }

    LogGraphicsSettings();

    // Basic OpenGL runtime settings
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Perspective display
    perspectiveMatrix = vmath::perspective(Version::FOV_Y, Version::ASPECT, Version::NEAR_PLANE, Version::FAR_PLANE);

    // Shaders
    AsteroidaGraphica::Log->Log("Shader creation...");
    if (!shaderManager.CreateShaderProgram("render", &flatShaderProgram))
    {
        return Version::Status::BAD_SHADERS;
    }

    if (!shaderManager.CreateShaderProgram("texRender", &textureShaderProgram))
    {
        return Version::Status::BAD_SHADERS;
    }

    if (!shaderManager.CreateShaderProgram("flatTexRender", &flatTextureShaderProgram))
    {
        return Version::Status::BAD_SHADERS;
    }

    mv_location = glGetUniformLocation(flatShaderProgram, "mv_matrix");
    proj_location = glGetUniformLocation(flatShaderProgram, "proj_matrix");
    mvTexLocation = glGetUniformLocation(textureShaderProgram, "mv_matrix");
    projTexLocation = glGetUniformLocation(textureShaderProgram, "proj_matrix");
    projFlatTexLocation = glGetUniformLocation(flatTextureShaderProgram, "proj_matrix");

    AsteroidaGraphica::Log->Log("Shader creation done!");

    // Images
    AsteroidaGraphica::Log->Log("Image loading...");
    GLuint compassTexture = imageManager.AddImage("images/DirectionDial.png");
    if (compassTexture == 0)
    {
        return Version::Status::BAD_IMAGES;
    }

    AsteroidaGraphica::Log->Log("Image loading done!");


    // FONT TEST CODE
    /*if (!displayFont.loadFromFile("fonts/DejaVuSans.ttf"))
    {
        return Version::Status::BAD_FONT;
    }*/

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

    shipHud.Initialize(compassTexture, projTexLocation, mvTexLocation);
    
    return Version::Status::OK;
}

Version::Status AsteroidaGraphica::Run()
{
    // 24 depth bits, 8 stencil bits, 8x AA, major version 3.
    AsteroidaGraphica::Log->Log("Graphics Initializing...");
    sf::ContextSettings contextSettings = sf::ContextSettings(24, 8, 8, 3, 0, 0);
    sf::Window window(sf::VideoMode(1280, 720), Version::NAME, sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close, contextSettings);
    Version::Status firstTimeSetup = LoadFirstTimeGraphics();
    if (firstTimeSetup != Version::Status::OK)
    {
        return firstTimeSetup;
    }

    UpdatePerspective(window.getSize().x, window.getSize().y);
    AsteroidaGraphica::Log->Log("Graphics Initialized!");
    
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
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                paused = false;
                physicsManager.Resume();
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
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    for (int k = 0; k < 5; k++)
                    {
                        vmath::mat4 mv_matrix = vmath::translate(vmath::vec3(i*separation, j*separation, k*separation));
                        glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
                        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
                    }
                }
            }

            // Draws our HUD dials
            glUseProgram(textureShaderProgram);
            shipHud.UpdateCompassRotations(physicsManager.shipOrientation.asEulerAngles());
            shipHud.RenderHud(perspectiveMatrix, clock);

            window.display();
        }

        sf::Int64 sleepDelay = (1000000 / Version::MAX_FRAMERATE) - clock.getElapsedTime().asMicroseconds() - clockStartTime.asMicroseconds();
        if (sleepDelay > 0)
        {
            std::chrono::microseconds sleepTime(sleepDelay);
            std::this_thread::sleep_for(sleepTime);
        }
    }


    return Version::Status::OK;
}

void AsteroidaGraphica::Deinitialize()
{
    // Application shutdown.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &pointBuffer);

    AsteroidaGraphica::Log->Log("Physica Thread Stopping...");
    physicsManager.Stop();
    physicaThread.wait();
    AsteroidaGraphica::Log->Log("Physica Thread Stopped.");
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
        asterioidaGraphica->Deinitialize();
    }
    else
    {
        AsteroidaGraphica::Log->Log(Logger::ERR, "Could not initialize the main program!");
    }

    // Wait before closing for display purposes.
    AsteroidaGraphica::Log->Log("Application End!");
    std::chrono::milliseconds sleepTime(1000);
    std::this_thread::sleep_for(sleepTime);

    // Log is auto-shutdown.
    return (int)runStatus;
}