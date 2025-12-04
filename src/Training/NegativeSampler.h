#pragma once

#include <vector>
#include <random>

class NegativeSampler
{
public:
    NegativeSampler(const std::unordered_map<int, int> &wordCounts, const int negativeSampleSize, double power = 0.75);

public:
    std::vector<int> sample(int targetIndex);

private:
    std::discrete_distribution<int> _distribution;
    std::mt19937 _generator;
    int _negativeSampleSize;
};