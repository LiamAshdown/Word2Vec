#include "Vocabulary/Vocabulary.h"

Vocabulary::Vocabulary(const TextProcessor &processor)
    : _processor(processor), _nextIndex(0), _totalWordCount(0)
{
}

void Vocabulary::buildFromTokens(const std::vector<std::string> &tokens)
{
    for (const std::string &word : tokens)
    {
        addWord(word);
    }
}

void Vocabulary::addWord(const std::string &word)
{
    std::string normalized = _processor.normalizeWord(word);

    if (!contains(normalized))
    {
        _wordToIndex[normalized] = _nextIndex;
        _indexToWord[_nextIndex] = normalized;
        _wordCounts[_nextIndex] = 0;
        _nextIndex++;
    }

    _wordCounts[_wordToIndex[normalized]]++;
    _totalWordCount++;
}

int Vocabulary::getIndex(const std::string &word) const
{
    std::string normalized = _processor.normalizeWord(word);
    auto it = _wordToIndex.find(normalized);
    if (it != _wordToIndex.end())
    {
        return it->second;
    }

    throw std::runtime_error("Word not found in vocabulary: " + normalized);
}

std::string Vocabulary::getWord(int index) const
{
    auto it = _indexToWord.find(index);
    if (it != _indexToWord.end())
    {
        return it->second;
    }
    throw std::runtime_error("Index not found in vocabulary: " + std::to_string(index));
}

bool Vocabulary::contains(const std::string &word) const
{
    std::string normalized = _processor.normalizeWord(word);
    return _wordToIndex.find(normalized) != _wordToIndex.end();
}

std::size_t Vocabulary::size() const
{
    return _wordToIndex.size();
}

std::vector<std::string> Vocabulary::getWords() const
{
    std::vector<std::string> words;
    words.reserve(_wordToIndex.size());
    for (const auto &pair : _wordToIndex)
    {
        words.push_back(pair.first);
    }
    return words;
}

const std::unordered_map<std::string, int> &Vocabulary::getWordToIndexMap() const
{
    return _wordToIndex;
}

std::unordered_map<int, int> Vocabulary::getWordCounts() const
{
    return _wordCounts;
}

int Vocabulary::getTotalWordCount() const
{
    return _totalWordCount;
}