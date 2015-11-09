#pragma once

// Manages object physics.
class Physica
{
    bool isAlive;
public:
    Physica();
    void Run();
    void Stop();
    ~Physica();
};

