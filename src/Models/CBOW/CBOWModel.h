#pragma once

#include "Models/Word2VecModel.h"

class CBOWModel : public Word2VecModel
{
public:
    CBOWModel(int vocabSize, int embeddingDim);

public:
    std::vector<double> forwardPass(const std::vector<int> &contextIndices) override;
    void backwardPass(const std::vector<int> &contextIndices, int targetIndex, std::vector<double> &probabilities, double learningRate) override;

    void backwardPassSampling(const std::vector<int> &contextIndices, int targetIndex, NegativeSampler &negativeSampler, double learningRate) override;

    std::string predictWord(const std::vector<std::string> &contextWords, const std::unordered_map<std::string, int> &vocab);

private:
    std::vector<double> _hidden;
};