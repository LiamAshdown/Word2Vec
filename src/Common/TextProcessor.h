#pragma once

#include <string>
#include <unordered_set>

class TextProcessor
{
public:
    TextProcessor();

public:
    void setLowercase(bool lowercase) { _lowercase = lowercase; }
    void setMinWordLength(size_t len) { _minWordLength = len; }
    void addStopWord(const std::string &word);
    void setStopWords(const std::unordered_set<std::string> &words);

public:
    std::string normalizeWord(const std::string &word) const;
    bool isValidWord(const std::string &word) const;

public:
    static std::string toLowercase(const std::string &text);
    static bool isWhitespace(char c);
    static bool isPunctuation(char c);

private:
    bool _lowercase;
    size_t _minWordLength;
    std::unordered_set<std::string> _stopWords;

private:
    bool isStopWord(const std::string &word) const;
};