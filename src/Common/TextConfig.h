#pragma once

#include <string>
#include <unordered_set>

class TextConfig
{
public:
    TextConfig();

public:
    void setLowercase(bool lowercase);
    void setMinWordLength(size_t len);
    void addStopWord(const std::string &word);
    void setStopWords(const std::unordered_set<std::string> &words);

public:
    bool isLowercase() const;
    size_t getMinWordLength() const;
    bool isStopWord(const std::string &word) const;
    const std::unordered_set<std::string> &getStopWords() const;

private:
    bool _lowercase;
    size_t _minWordLength;
    std::unordered_set<std::string> _stopWords;
};
