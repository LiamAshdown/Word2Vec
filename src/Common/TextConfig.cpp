#include "Common/TextConfig.h"

TextConfig::TextConfig()
    : _lowercase(true),
      _minWordLength(1)
{
}

void TextConfig::setLowercase(bool lowercase)
{
    _lowercase = lowercase;
}

void TextConfig::setMinWordLength(size_t len)
{
    _minWordLength = len;
}

void TextConfig::addStopWord(const std::string &word)
{
    _stopWords.insert(word);
}

void TextConfig::setStopWords(const std::unordered_set<std::string> &words)
{
    _stopWords = words;
}

bool TextConfig::isLowercase() const
{
    return _lowercase;
}

size_t TextConfig::getMinWordLength() const
{
    return _minWordLength;
}

bool TextConfig::isStopWord(const std::string &word) const
{
    return _stopWords.find(word) != _stopWords.end();
}

const std::unordered_set<std::string> &TextConfig::getStopWords() const
{
    return _stopWords;
}