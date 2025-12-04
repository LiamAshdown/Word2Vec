#include "TrainingDataBuilder.h"
#include "Common/Random/RandomNumber.h"

TrainingDataBuilder::TrainingDataBuilder(int windowSize)
    : windowSize(windowSize) {}

std::vector<ContextWindow> TrainingDataBuilder::createWindows(const std::vector<int> &tokenIndices, const std::unordered_map<int, int> &wordCounts, const int totalWordCount) const
{
    std::vector<ContextWindow> windows;

    if (tokenIndices.size() < 2)
    {
        return windows; // Not enough tokens to create windows
    }

    const double threshold = 1e-5;

    // For each position in the sequence (except at boundaries)
    for (size_t i = 0; i < tokenIndices.size(); ++i)
    {
        double frequency = static_cast<double>(wordCounts.at(tokenIndices[i])) / totalWordCount;

        double discardProbability = 1.0 - sqrt(threshold / frequency);
        double randomValue = RandomNumber::Instance()->Number<double>(0.0, 1.0);

        if (randomValue < discardProbability)
        {
            continue;
        }

        ContextWindow window;
        window.centerIndex = tokenIndices[i];

        // Collect context words within the window
        for (int j = -windowSize; j <= windowSize; ++j)
        {
            if (j == 0)
                continue; // Skip the center word itself

            int contextPos = i + j;
            if (contextPos >= 0 && contextPos < tokenIndices.size())
            {
                window.contextIndices.push_back(tokenIndices[contextPos]);
            }
        }

        // Only add window if it has context
        if (!window.contextIndices.empty())
        {
            windows.push_back(window);
        }
    }

    return windows;
}

void TrainingDataBuilder::setWindowSize(int size)
{
    windowSize = size;
}

int TrainingDataBuilder::getWindowSize() const
{
    return windowSize;
}
