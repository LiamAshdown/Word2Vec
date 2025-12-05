#include "Tokenizer/Tokenizer.h"

Tokenizer::Tokenizer(const TextProcessor &processor)
    : _processor(processor), _tokenCount(0)
{
}

std::vector<std::string> Tokenizer::tokenize(FileStream &fileStream)
{
    std::vector<std::string> tokens;
    std::string current;

    auto flushCurrent = [&]()
    {
        flush(current, tokens);
    };

    std::vector<char> buffer(1);

    while (!fileStream.eof())
    {
        std::size_t bytesRead = fileStream.read(buffer.data(), buffer.size());

        for (std::size_t i = 0; i < bytesRead; ++i)
        {
            const char character = buffer[i];

            process(current, character, tokens);
        }
    }

    flush(current, tokens);

    return tokens;
}

std::vector<std::string> Tokenizer::tokenize(std::string words)
{
    std::vector<std::string> tokens;
    std::string current;

    for (std::size_t i = 0; i < words.length(); i++)
    {
        const char character = words[i];

        process(current, character, tokens);
    }
}

void Tokenizer::process(std::string &current, const char character, std::vector<std::string> &tokens)
{
    if (TextProcessor::isWhitespace(character))
    {
        flush(current, tokens);
    }
    else if (TextProcessor::isPunctuation(character))
    {
        flush(current, tokens);

        std::string punct(1, character);
        std::string normalized = _processor.normalizeWord(punct);
        if (_processor.isValidWord(normalized))
        {
            tokens.push_back(normalized);
            _tokenCount++;
        }
    }
    else
    {
        current += character;
    }
}

void Tokenizer::flush(std::string &current, std::vector<std::string> &tokens)
{
    if (!current.empty())
    {
        std::string normalized = _processor.normalizeWord(current);

        if (_processor.isValidWord(normalized))
        {
            tokens.push_back(normalized);
            _tokenCount++;
        }

        current.clear();
    }
}

std::size_t Tokenizer::getTokenCount() const
{
    return _tokenCount;
}
