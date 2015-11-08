#pragma once

#include <ctime>
#include <fstream>
#include <SFML\System.hpp>

#include "Version.h"

// A simple class for logging program events out to a file.
class Logger : sf::NonCopyable
{
public:
    enum LogType { DEBUG, INFO, WARN, ERROR };

private:
    std::ofstream logFile;

    // Logs the current time out to the log file.
    void LogTime()
    {
        time_t currentTime = time(nullptr);
        struct tm localTime;
        localtime_s(&localTime, &currentTime);
        logFile << "[" << (localTime.tm_year + 1900) << "-" << (localTime.tm_mon + 1) << "-" << localTime.tm_mday
            << " " << localTime.tm_hour << ":" << localTime.tm_min << ":" << localTime.tm_sec << "] ";
    }

    // Retrieve the log type given the enumeration.
    const char* GetLogType(LogType logType)
    {
        switch (logType)
        {
        case LogType::DEBUG:
            return "debug: ";
        case LogType::INFO:
            return "info: ";
        case LogType::WARN:
            return "WARNING: ";
        case LogType::ERROR:
            return "ERROR: ";
        }

        return "EXTREME ERROR: ";
    }

public:
    // Creates and logs the startup text
    Logger(const char* fileName)
    {
        logFile.open(fileName);

        if (logFile.is_open())
        {
            logFile << Version::NAME << " " << Version::MAJOR_VERSION << "." << Version::MINOR_VERSION << std::endl << std::endl;
        }
    }

    // Logs an informational message
    void Log(const char* message)
    {
        Log(LogType::INFO, message);
    }

    // Logs a message out the logger
    void Log(LogType logType, const char* message)
    {
        LogTime();
        logFile << GetLogType(logType) << message << std::endl;
    }

    // Destructs the logger
    ~Logger()
    {
        if (logFile.is_open())
        {
            logFile << std::endl;
            logFile.close();
        }
    }
};