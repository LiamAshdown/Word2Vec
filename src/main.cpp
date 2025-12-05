#include "Common/TextProcessor.h"
#include "Tokenizer/Tokenizer.h"
#include "Vocabulary/Vocabulary.h"
#include "Models/CBOW/CBOWModel.h"
#include "Training/Trainer.h"
#include "Common/Logger/ConsoleLogger.h"
#include "Common/File/FileStream.h"
#include <iostream>
#include <memory>

int main()
{
    FileStream fileStream("./corpus/text.txt", FileStream::Mode::Read);

    TextProcessor processor;
    processor.setLowercase(true);
    processor.setMinWordLength(2);

    Tokenizer tokenizer(processor);
    Vocabulary vocab(processor);

    auto tokens = tokenizer.tokenize(fileStream);
    vocab.buildFromTokens(tokens);

    std::cout << "Vocabulary size: " << vocab.size() << std::endl;

    std::vector<int> tokenIndices;
    for (const auto &token : tokens)
    {
        tokenIndices.push_back(vocab.getIndex(token));
    }

    int embeddingDim = 128;
    auto model = std::make_shared<CBOWModel>(vocab.size(), embeddingDim);

    auto logger = std::make_shared<ConsoleLogger>();

    Trainer trainer(model, vocab, 0.01, logger.get());

    // trainer.train(tokenIndices, 10000, 2);
    trainer.trainWithNegativeSampling(tokenIndices, 100, 4, 5);

    int foxIndex = vocab.getIndex("fox");
    std::vector<std::string> contextWords = tokenizer.tokenize("brown fox");

    std::string predictedWord = model->predictWord(contextWords, vocab.getWordToIndexMap());
    logger->info("Predicted word for context [quick, brown]: " + predictedWord);

    std::vector<double> embeddings = model->generateEmbeddings(contextWords, vocab.getWordToIndexMap());

    return 0;
}