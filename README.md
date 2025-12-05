# Word2Vec

This is an implementation of [Word2Vec](https://en.wikipedia.org/wiki/Word2vec) using [CBOW](https://www.geeksforgeeks.org/nlp/continuous-bag-of-words-cbow-in-nlp/). This was built from scratch; only the C++ STD library is used.

### How does it work?

#### Tokenization

Reads the corpus data (the training data) and converts it into individual tokens (words).
For example, take: "The quick brown fox" - the tokenizer will convert this into an array: `["the", "quick", "brown", "fox"]`.

The tokenizer also converts the words into lowercase, to help our training model predict better.

#### Vocabulary

Once we have our tokens, we need to pass this into our Vocabulary class. The Vocabulary class is responsible for keeping track of the following:

- `wordToIndex` -> `O(1)` lookup
- `indexToWord` -> `O(1)` lookup
- `wordCounts` -> How many times this word appeared in our corpus
- `totalWordCount` -> How many (not unique) words exist in the corpus

Each word is assigned a unique index (or you can call it ID).

#### Training

Once the Tokenizer has extracted the corpus into tokens, and the Vocabulary stores statistics about the tokens, we can now proceed to the next step, training.

I will break this into steps:

**1. Context Windows**

CBOW works by taking a context, and attempts to predict the target word.

Let's simulate a context window.

Take the following sentence "the quick brown fox jumps over the lazy dog."

The context window will create the following windows:

- `["the", "brown"]` = `"quick"`
- `["quick", "fox"]` = `"brown"`
- `["brown", "jumps"]` = `"fox"`

The array is the context words, and the value after the equals is the center/target word we're trying to predict.

During our training, it would go through the windows, and the model will take the context, and predict what word would best fit in the context? If the model got it wrong, we give the model the correct target word, so it will remember next time that context appears, it would correctly predict the word for the context.

Once we have our context windows, we can now begin the training.

To train we have the following properties:

- W1 -> Input embeddings (this becomes our final trained word embeddings)
- W2 -> Output weights that map from hidden layer to vocabulary predictions
- Hidden layer -> The context of the words averaged.

First we begin by creating random values for our W1 and W2 and this is an array of numbers based from the dimensions used (more dimensions, the more meaning a word can have).

At this point our model is completely clueless - we need to train it.

We loop through our context windows as described earlier, and pass in the context of words to the model.

The model will then use those words to create the hidden layer, by summing the words (the embeddings) and creating an average - this creates the meaning of the context words together.

Then we go through each word in our vocabulary and calculate the probability that the word would be the target word in the context given.

Once all the words in our vocabulary has the predicted probability that the word would be the target word in the context.

For example, let's say the context is: `["the", "quick", "brown"]` = `"fox"`,

and we predicted the following words with their probabilities:

- "the" -> 0.5
- "quick" -> 0.3
- "brown" -> 0.8
- "fox" -> 0.1

We inform the model the target word is fox, and to do this we subtract 1 from the target word's probability: `0.1 - 1.0 = -0.9` (We update the probability again just for the target word). If the value is negative, we need to increase the probability (go up), if it's positive, we need to decrease the probability (go down).

Now we have our probabilities with our target word adjusted. We now need to calculate the hidden gradient:

```
hiddenGrad[dim] = sum over all words (probabilities[wordIndex] * W2[dim][wordIndex])
```

We add all vocab words together multiplied by its probability. This is used to update our context words later. So the next time the same context window appears in the next iteration, it will have a better understanding what word should be the most likely to be predicted.

Now, we can update the W2 weights for all of our vocab words based from the context.

```
W2[dim][wordIndex] -= learningRate * probabilities[wordIndex] * hidden[dim]
```

If we predicted too high for this word (positive error), we decrease its weights. If we predicted too low for this word (negative error), we increase its weights for this context. This is how we create relationships between words and their context.

Over the course of many iterations of training, you start to see the word "dog" moving closer to contexts that talks about animals etc..

Finally, we update the W1 weights based on what we learnt with this context window.

```
W1[contextWordIndex][dim] -= (learningRate / contextSize) * hiddenGrad[dim]
```

This is basically saying to each context word: "The next time I appear in this same context, I should have a better guess on what the target word is from the hidden gradient (the gradient that stores the errors)".

I hope this makes sense. This was written in a way I could understand it without having to re-read the technical implementation.