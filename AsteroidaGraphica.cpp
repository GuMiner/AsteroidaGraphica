#include "AsteroidaGraphica.h"

#pragma comment(lib, "opengl32")

#ifndef _DEBUG
#pragma comment(lib, "lib/sfml-system")
#else
#pragma comment(lib, "lib/sfml-system-d")
#endif

std::unique_ptr<Logger> AsteroidaGraphica::Log;

AsteroidaGraphica::AsteroidaGraphica()
{
}

Version::Status AsteroidaGraphica::Initialize()
{
    return Version::Status::OK;
}

Version::Status AsteroidaGraphica::Run()
{
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
        AsteroidaGraphica::Log->Log(Logger::ERROR, "Could not initialize the logger!");
    }

    // Wait before closing for display purposes.
    AsteroidaGraphica::Log->Log("Application End!");
    std::chrono::milliseconds sleepTime(2000);
    std::this_thread::sleep_for(sleepTime);

    // Log is auto-shutdown.
    return (int)runStatus;
}