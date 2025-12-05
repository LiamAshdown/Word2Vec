#include "Models/Word2VecModel.h"

#include <cmath>
#include <random>
#include <stdexcept>

Word2VecModel::Word2VecModel(int vocabSize, int embeddingDim)
    : _vocabSize(vocabSize), _embeddingDim(embeddingDim)
{
    initializeWeights();
}

void Word2VecModel::initializeWeights()
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-0.5 / _embeddingDim, 0.5 / _embeddingDim);

    W1.resize(_vocabSize, std::vector<double>(_embeddingDim));
    W2.resize(_embeddingDim, std::vector<double>(_vocabSize));

    for (int i = 0; i < _vocabSize; i++)
    {
        for (int j = 0; j < _embeddingDim; j++)
        {
            W1[i][j] = distribution(generator);
        }
    }

    for (int i = 0; i < _embeddingDim; i++)
    {
        for (int j = 0; j < _vocabSize; j++)
        {
            W2[i][j] = distribution(generator);
        }
    }
}

std::vector<double> Word2VecModel::getWordEmbedding(int wordIndex) const
{
    if (wordIndex < 0 || wordIndex >= _vocabSize)
    {
        throw std::out_of_range("Word index out of range");
    }

    return W1[wordIndex];
}

std::vector<double> Word2VecModel::softmax(const std::vector<double> &scores) const
{
    std::vector<double> expScores(scores.size());
    double maxScore = *std::max_element(scores.begin(), scores.end());
    double sumExp = 0.0;

    for (std::size_t i = 0; i < scores.size(); i++)
    {
        expScores[i] = std::exp(scores[i] - maxScore);
        sumExp += expScores[i];
    }

    for (double &val : expScores)
    {
        val /= sumExp;
    }

    return expScores;
}
