#include "Models/CBOW/CBOWModel.h"

CBOWModel::CBOWModel(int vocabSize, int embeddingDim)
    : Word2VecModel(vocabSize, embeddingDim), _hidden(embeddingDim, 0.0)
{
}

void CBOWModel::pass(ContextWindow window, double learningRate)
{
    std::vector<double> probabilities = forwardPass(window.contextIndices);

    backwardPass(window.contextIndices, window.centerIndex, probabilities, learningRate);
}

void CBOWModel::passSampling(ContextWindow window, NegativeSampler &negativeSampler, double learningRate)
{
    std::vector<double> hiddenGrad(_embeddingDim, 0.0);

    auto updateForSample = [&](int wordIndex, double targetLabel)
    {
        double score = 0.0;
        for (int j = 0; j < _embeddingDim; j++)
        {
            score += _hidden[j] * W2[j][wordIndex];
        }

        double prediction = 1.0 / (1.0 + std::exp(-score)); // Sigmoid
        double error = prediction - targetLabel;

        for (int j = 0; j < _embeddingDim; j++)
        {
            hiddenGrad[j] += error * W2[j][wordIndex];
            W2[j][wordIndex] -= learningRate * error * _hidden[j];
        }
    };

    updateForSample(window.centerIndex, 1.0);

    std::vector<int> negativeSamples = negativeSampler.sample(window.centerIndex);
    for (int negativeIndex : negativeSamples)
    {
        updateForSample(negativeIndex, 0.0);
    }

    double contextScale = learningRate / window.contextIndices.size();
    for (int idx : window.contextIndices)
    {
        for (int j = 0; j < _embeddingDim; j++)
        {
            W1[idx][j] -= contextScale * hiddenGrad[j];
        }
    }
}

std::vector<double> CBOWModel::forwardPass(const std::vector<int> &contextIndices)
{
    std::fill(_hidden.begin(), _hidden.end(), 0.0);

    // Average the context word embeddings
    for (int idx : contextIndices)
    {
        for (int j = 0; j < _embeddingDim; j++)
        {
            // Sum up all the context word embeddings
            _hidden[j] += W1[idx][j];
        }
    }

    // Now average the hidden layer values
    // This "normalizes" the hidden layer so its values don't grow too large based on the number of context words
    for (int j = 0; j < _embeddingDim; j++)
    {
        _hidden[j] /= contextIndices.size();
    }

    // Compute output scores for every word in vocabulary
    // For each word, calculate how well the context pattern matches that word's pattern
    std::vector<double> outputScores(_vocabSize, 0.0);

    for (int wordIndex = 0; wordIndex < _vocabSize; wordIndex++)
    {
        double wordScore = 0.0;

        for (int dimension = 0; dimension < _embeddingDim; dimension++)
        {
            double contextFeature = _hidden[dimension];
            double wordFeature = W2[dimension][wordIndex];

            // 0.9 × 0.9 = 0.81 → Strong match (both high, same direction)
            // 0.1 × 0.1 = 0.01 → Weak match (same direction but small values)
            // -0.5 × -0.5 = 0.25 → Moderate match (both negative = same direction!)
            // 0.5 × -0.5 = -0.25 → Moderate mismatch (opposite directions)
            // 0.0 × anything = 0 → No relationship
            wordScore += contextFeature * wordFeature;
        }

        // how close are we to predicting this word given the context?
        outputScores[wordIndex] = wordScore;
    }

    return outputScores;
}

void CBOWModel::backwardPass(const std::vector<int> &contextIndices, int targetIndex, std::vector<double> &probabilities, double learningRate)
{
    std::vector<double> outputGrad = probabilities;

    // Check how wrong we are, if negative we go up, if positive we go down
    outputGrad[targetIndex] -= 1.0;

    {
        std::vector<double> hiddenGrad(_embeddingDim, 0.0);
        for (int j = 0; j < _embeddingDim; j++)
        {
            for (int k = 0; k < _vocabSize; k++)
            {
                // hidden[dim] = sum over k (outputGrad[wordindex] * W2[dim][wordindex])
                // hiddenGrad represents the total error signal each dimension
                hiddenGrad[j] += outputGrad[k] * W2[j][k];
            }
        }

        // _hidden = your current location
        // hiddenGrad = the direction and distance you need to move to reach your destination

        for (int j = 0; j < _embeddingDim; j++)
        {
            for (int k = 0; k < _vocabSize; k++)
            {
                // Adjust the W2 to be closer or further from the hidden layer (the context) based on the output error
                // If we predicted too high for this word, decrease its weights (move away)
                // If we predicted too low for this word, increase its weights (move closer)
                W2[j][k] -= learningRate * outputGrad[k] * _hidden[j];
            }
        }

        // Since the forward pass averages the context, we need to scale the gradient by the same factor
        double contextScale = learningRate / contextIndices.size();
        for (int idx : contextIndices)
        {
            for (int j = 0; j < _embeddingDim; j++)
            {
                // This is basically saying to each context word:
                // The next time I appear in this same context,
                // I should have a better guess on what the target word is
                W1[idx][j] -= contextScale * hiddenGrad[j];
            }
        }
    }
}

std::string CBOWModel::predictWord(const std::vector<std::string> &contextWords, const std::unordered_map<std::string, int> &vocab)
{
    std::vector<int> contextIndices;

    for (const std::string &word : contextWords)
    {
        // Fixed window size
        if (contextIndices.size() > 2)
        {
            break;
        }

        if (vocab.find(word) != vocab.end())
        {
            contextIndices.push_back(vocab.at(word));
        }
    }

    if (contextIndices.empty())
    {
        return "";
    }

    auto outputScores = forwardPass(contextIndices);
    auto probabilities = softmax(outputScores);

    int maxIdx = 0;
    double maxProb = probabilities[0];
    for (std::size_t i = 1; i < probabilities.size(); i++)
    {
        if (probabilities[i] > maxProb)
        {
            maxProb = probabilities[i];
            maxIdx = i;
        }
    }

    // Return the word corresponding to the highest probability
    for (const auto &pair : vocab)
    {
        if (pair.second == maxIdx)
        {
            return pair.first;
        }
    }

    return "";
}

std::vector<double> CBOWModel::generateEmbeddings(const std::vector<std::string> &contextWords, const std::unordered_map<std::string, int> &vocab)
{
    std::vector<double> combinedEmbeddings(_embeddingDim, 0);
    for (const auto &word : contextWords)
    {
        int index = vocab.at(word);
        const auto &embedding = W1[index];

        for (int i = 0; i < _embeddingDim; i++)
        {
            combinedEmbeddings[i] += embedding[i];
        }
    }

    for (int i = 0; i < _embeddingDim; i++)
    {
        combinedEmbeddings[i] /= contextWords.size();
    }

    return combinedEmbeddings;
}