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
    };

    std::vector<char> buffer(1);

    while (!fileStream.eof())
    {
        std::size_t bytesRead = fileStream.read(buffer.data(), buffer.size());

        for (std::size_t i = 0; i < bytesRead; ++i)
        {
            const char character = buffer[i];

            if (TextProcessor::isWhitespace(character))
            {
                flushCurrent();
            }
            else if (TextProcessor::isPunctuation(character))
            {
                flushCurrent();

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
    }

    flushCurrent();

    return tokens;
}

std::size_t Tokenizer::getTokenCount() const
{
    return _tokenCount;
}
