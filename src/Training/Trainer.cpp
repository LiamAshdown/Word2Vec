#include "Trainer.h"
#include "../Common/Logger/NullLogger.h"
#include <iostream>
#include <cmath>

Trainer::Trainer(std::shared_ptr<Word2VecModel> model, const Vocabulary vocabulary, double learningRate, Logger *logger)
	: model(model), learningRate(learningRate), _vocabulary(vocabulary), logger(logger ? logger : new NullLogger()) {}

void Trainer::train(const std::vector<int> &tokenIndices, int epochs, int windowSize)
{
	TrainingDataBuilder builder(windowSize);
	std::vector<ContextWindow> windows = builder.createWindows(tokenIndices, _vocabulary.getWordCounts(), _vocabulary.getTotalWordCount());

	if (windows.empty())
	{
		logger->error("No training windows created. Check your data.");
		return;
	}

	logger->info("Training on " + std::to_string(windows.size()) + " windows for " + std::to_string(epochs) + " epochs...");
	for (int epoch = 0; epoch < epochs; ++epoch)
	{
		for (const auto &window : windows)
		{
			std::vector<double> predictions = model->forwardPass(window.contextIndices);
			model->backwardPass(window.contextIndices, window.centerIndex, predictions, learningRate);
		}

		// Print progress
		if ((epoch + 1) % 10 == 0 || epoch == 0)
		{
			logger->info("Epoch " + std::to_string(epoch + 1) + "/" + std::to_string(epochs) + " - Training step complete");
		}
	}

	logger->info("Training complete!");
}

void Trainer::trainWithNegativeSampling(const std::vector<int> &tokenIndices,
										int epochs,
										int windowSize,
										int numNegativeSamples)
{
	TrainingDataBuilder builder(windowSize);
	std::vector<ContextWindow> windows = builder.createWindows(tokenIndices, _vocabulary.getWordCounts(), _vocabulary.getTotalWordCount());

	if (windows.empty())
	{
		logger->error("No training windows created. Check your data.");
		return;
	}

	NegativeSampler negativeSampler(_vocabulary.getWordCounts(), numNegativeSamples);

	logger->info("Training with negative sampling (" + std::to_string(numNegativeSamples) +
				 " samples) on " + std::to_string(windows.size()) +
				 " windows for " + std::to_string(epochs) + " epochs...");

	for (int epoch = 0; epoch < epochs; ++epoch)
	{
		for (const auto &window : windows)
		{
			model->backwardPassSampling(window.contextIndices,
										window.centerIndex,
										negativeSampler,
										learningRate);
		}

		if ((epoch + 1) % 10 == 0 || epoch == 0)
		{
			logger->info("Epoch " + std::to_string(epoch + 1) + "/" +
						 std::to_string(epochs) + " - Training step complete");
		}
	}

	logger->info("Training with negative sampling complete!");
}

void Trainer::setLearningRate(double rate)
{
	learningRate = rate;
}

double Trainer::getLearningRate() const
{
	return learningRate;
}