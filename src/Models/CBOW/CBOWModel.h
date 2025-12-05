#pragma once

#include "Models/Word2VecModel.h"

class CBOWModel : public Word2VecModel
{
public:
    CBOWModel(int vocabSize, int embeddingDim);

public:
    void pass(ContextWindow window, double learningRate) override;
    void passSampling(ContextWindow window, NegativeSampler &negativeSampler, double learningRate) override;

    std::string predictWord(const std::vector<std::string> &contextWords, const std::unordered_map<std::string, int> &vocab) override;

private:
    void backwardPass(const std::vector<int> &contextIndices, int targetIndex, std::vector<double> &probabilities, double learningRate);
    std::vector<double> forwardPass(const std::vector<int> &contextIndices);

private:
    std::vector<double> _hidden;
};