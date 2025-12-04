#include "Logger.h"

class NullLogger : public Logger
{
public:
    void info(const std::string &message) override {}
    void warning(const std::string &message) override {}
    void error(const std::string &message) override {}
};