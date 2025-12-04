#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Common/TextProcessor.h"

class Vocabulary
{
public:
    Vocabulary(const TextProcessor &processor);

public:
    void buildFromTokens(const std::vector<std::string> &tokens);

    void addWord(const std::string &word);

public:
    int getIndex(const std::string &word) const;
    std::string getWord(int index) const;
    bool contains(const std::string &word) const;
    std::size_t size() const;

    std::vector<std::string> getWords() const;
    const std::unordered_map<std::string, int> &getWordToIndexMap() const;
    std::unordered_map<int, int> getWordCounts() const;
    int getTotalWordCount() const;

private:
    const TextProcessor &_processor;
    std::unordered_map<std::string, int> _wordToIndex;
    std::unordered_map<int, std::string> _indexToWord;
    std::unordered_map<int, int> _wordCounts;
    int _totalWordCount;

    int _nextIndex;
};