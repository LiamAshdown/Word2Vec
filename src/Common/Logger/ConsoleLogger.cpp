#include "Common/Logger/ConsoleLogger.h"

void ConsoleLogger::info(const std::string &message)
{
    std::lock_guard<std::mutex> lock(_logMutex);
    std::cout << "[INFO]: " << message << std::endl;
}

void ConsoleLogger::warning(const std::string &message)
{
    std::lock_guard<std::mutex> lock(_logMutex);
    std::cout << "[WARNING]: " << message << std::endl;
}

void ConsoleLogger::error(const std::string &message)
{
    std::lock_guard<std::mutex> lock(_logMutex);
    std::cerr << "[ERROR]: " << message << std::endl;
}