#pragma once

#include "Common/TextProcessor.h"
#include "Common/File/FileStream.h"

#include <string>
#include <vector>

class Tokenizer
{
public:
    Tokenizer(const TextProcessor &processor);

    std::vector<std::string> tokenize(FileStream &fileStream);

    std::size_t getTokenCount() const;

private:
    const TextProcessor &_processor;
    std::size_t _tokenCount;
};