#include "Training/NegativeSampler.h"

#include <cmath>
#include <unordered_set>

NegativeSampler::NegativeSampler(const std::unordered_map<int, int> &wordCounts, const int negativeSampleSize, double power)
{
    std::vector<double> probs;
    for (auto &it : wordCounts)
    {
        probs.push_back(std::pow(it.second, power));
    }

    _distribution = std::discrete_distribution<int>(probs.begin(), probs.end());
    _generator = std::mt19937(std::random_device{}());
    _negativeSampleSize = negativeSampleSize;
}

std::vector<int> NegativeSampler::sample(int targetIndex)
{
    std::vector<int> negatives;
    std::unordered_set<int> seen;
    seen.insert(targetIndex); // Don't sample the target

    while (negatives.size() < _negativeSampleSize)
    {
        int idx = _distribution(_generator);
        if (seen.find(idx) == seen.end())
        {
            negatives.push_back(idx);
            seen.insert(idx);
        }
    }
    return negatives;
}