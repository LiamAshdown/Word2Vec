#pragma once
#include <vector>
#include <string>

#include "../Vocabulary/Vocabulary.h"
#include "Training/NegativeSampler.h"

struct TrainingPair
{
    std::vector<int> contextIndices;
    int targetIndex;
};

class Word2VecModel
{
public:
    Word2VecModel(int vocabSize, int embeddingDim);
    virtual ~Word2VecModel() = default;

public:
    virtual std::vector<double> forwardPass(const std::vector<int> &contextIndices) = 0;
    virtual void backwardPass(const std::vector<int> &contextIndices, int targetIndex, std::vector<double> &probabilities, double learningRate) = 0;

    virtual void backwardPassSampling(const std::vector<int> &contextIndices, int targetIndex, NegativeSampler &negativeSampler, double learningRate) = 0;

public:
    std::vector<double> getWordEmbedding(int wordIndex) const;
    std::vector<double> softmax(const std::vector<double> &scores) const;
    std::string predictWord(const std::vector<std::string> &contextWords, const std::unordered_map<std::string, int> &vocab);

protected:
    void initializeWeights();

protected:
    int _vocabSize;
    int _embeddingDim;
    // W1: "How should I represent myself when I'm part of a context?"
    // W2: "What context pattern signals that I should appear as the target?"
    std::vector<std::vector<double>> W1;
    std::vector<std::vector<double>> W2;
};