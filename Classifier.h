#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <map>
#include <vector>
#include "DSString.h"
using namespace std;

struct SentimentWordCount{
    int positiveCount = 0;
    int negativeCount = 0;
};

vector<DSString> parseCSVLine(const DSString& line);

vector<DSString> tokenize(const DSString& text);

void trainClassifier(const string& filename, map<DSString, SentimentWordCount>& wordSentiments);

int classifyTweet(const DSString& tweet, const map<DSString, SentimentWordCount>& wordSentiments);

float calculateAccuracy(const string& resultsFileName, const string& groundTruthFilename, const string& accuracyFilename);


#endif
