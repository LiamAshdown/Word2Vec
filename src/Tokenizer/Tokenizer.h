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
    std::vector<std::string> tokenize(std::string words);

    std::size_t getTokenCount() const;

private:
    void flush(std::string &current, std::vector<std::string> &tokens);
    void process(std::string &current, const char character, std::vector<std::string> &tokens);

private:
    const TextProcessor &_processor;
    std::size_t _tokenCount;
};