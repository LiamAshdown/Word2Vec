#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

enum class LogType
{
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    virtual ~Logger() {};

public:
    virtual void info(const std::string &message) = 0;
    virtual void warning(const std::string &message) = 0;
    virtual void error(const std::string &message) = 0;

protected:
    std::mutex _logMutex;
};