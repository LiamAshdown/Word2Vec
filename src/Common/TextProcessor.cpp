#include "Common/TextProcessor.h"
#include <algorithm>
#include <cctype>

TextProcessor::TextProcessor()
    : _lowercase(true), _minWordLength(1)
{
}

void TextProcessor::addStopWord(const std::string &word)
{
    _stopWords.insert(normalizeWord(word));
}

void TextProcessor::setStopWords(const std::unordered_set<std::string> &words)
{
    _stopWords.clear();
    for (const auto &word : words)
    {
        _stopWords.insert(normalizeWord(word));
    }
}

std::string TextProcessor::normalizeWord(const std::string &word) const
{
    if (_lowercase)
    {
        return toLowercase(word);
    }
    return word;
}

bool TextProcessor::isValidWord(const std::string &word) const
{
    std::string normalized = normalizeWord(word);

    if (normalized.length() < _minWordLength)
    {
        return false;
    }

    if (isStopWord(normalized))
    {
        return false;
    }

    return true;
}

bool TextProcessor::isStopWord(const std::string &word) const
{
    std::string normalized = normalizeWord(word);
    return _stopWords.find(normalized) != _stopWords.end();
}

std::string TextProcessor::toLowercase(const std::string &text)
{
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

bool TextProcessor::isWhitespace(char c)
{
    return std::isspace(static_cast<unsigned char>(c));
}

bool TextProcessor::isPunctuation(char c)
{
    return std::ispunct(static_cast<unsigned char>(c));
}