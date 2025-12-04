#pragma once

#include <vector>
#include <unordered_map>

struct ContextWindow
{
    std::vector<int> contextIndices; // Surrounding words
    int centerIndex;                 // Center word
};

class TrainingDataBuilder
{
public:
    TrainingDataBuilder(int windowSize);

    std::vector<ContextWindow> createWindows(const std::vector<int> &tokenIndices, const std::unordered_map<int, int> &wordCounts, const int totalWordCount) const;

    void setWindowSize(int size);
    int getWindowSize() const;

private:
    int windowSize;
};
