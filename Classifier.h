#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <map>
#include <vector>
#include "DSString.h"
using namespace std;

struct SentimentWordCount{ // declares and defines the SentimentWordCount structure with initial values of 0.
    int positiveCount = 0;
    int negativeCount = 0;
};

vector<DSString> parseCSVLine(const DSString& line); // declares the parseCSVLine function to return a vector of DSString objects.

vector<DSString> tokenize(const DSString& text); // declares the tokenize function to return a vector of DSString objects.

void trainClassifier(const string& filename, map<DSString, SentimentWordCount>& wordSentiments); // declares the trainClassifier
                                                                                                 // function as a void function.

int classifyTweet(const DSString& tweet, const map<DSString, SentimentWordCount>& wordSentiments); // declares the classifyTweet
                                                                                                   // function to return an int value.

float calculateAccuracy(const string& resultsFileName, const string& groundTruthFilename, const string& accuracyFilename);
// declares the calculateAccuracy function to return a float value.


#endif
