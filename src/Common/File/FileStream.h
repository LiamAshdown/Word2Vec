#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

class FileStream
{
public:
    enum class Mode
    {
        Read,
        Write,
        ReadWrite,
        Append
    };

    FileStream(const std::string path, Mode mode);

public:
    std::size_t read(void *buffer, std::size_t size);
    bool eof() const;

private:
    void open(const std::string path, Mode mode);

private:
    std::fstream _stream;
};