#pragma once

#include "../Models/Word2VecModel.h"
#include "../Vocabulary/Vocabulary.h"
#include "TrainingDataBuilder.h"
#include "NegativeSampler.h"
#include "../Common/Logger/Logger.h"
#include <vector>
#include <memory>
#include <optional>

class Trainer
{
public:
    Trainer(std::shared_ptr<Word2VecModel> model, const Vocabulary Vocabulary, double learningRate, Logger *logger = nullptr);

    void train(const std::vector<int> &tokenIndices, const int epochs, int windowSize);
    void trainWithNegativeSampling(const std::vector<int> &tokenIndices,
                                   int epochs,
                                   int windowSize,
                                   int numNegativeSamples = 5);

    void setLearningRate(double rate);
    double getLearningRate() const;

private:
    std::shared_ptr<Word2VecModel> model;
    const Vocabulary _vocabulary;
    double learningRate;
    Logger *logger;
};
