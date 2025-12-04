#include "Common/File/FileStream.h"

FileStream::FileStream(const std::string path, Mode mode)
{
    open(path, mode);
}

void FileStream::open(const std::string path, Mode mode)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Could not find " + path);
    }

    std::ios_base::openmode openMode = std::ios::binary;

    switch (mode)
    {
    case Mode::Read:
        openMode |= std::ios::in;
        break;
    case Mode::Write:
        openMode |= std::ios::out | std::ios::trunc;
        break;
    case Mode::ReadWrite:
        openMode |= std::ios::in | std::ios::out;
        break;
    case Mode::Append:
        openMode |= std::ios::out | std::ios::app;
        break;
    }

    _stream.open(path, openMode);
}

std::size_t FileStream::read(void *buffer, std::size_t size)
{
    _stream.read(reinterpret_cast<char *>(buffer), size);

    return static_cast<std::size_t>(_stream.gcount());
}

bool FileStream::eof() const
{
    return _stream.eof();
}