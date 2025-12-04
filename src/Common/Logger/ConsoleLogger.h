#pragma once

#include "Common/Logger/Logger.h"

#include <iostream>
#include <string>

class ConsoleLogger : public Logger
{
public:
    void info(const std::string &message) override;
    void warning(const std::string &message) override;
    void error(const std::string &message) override;
};