#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include "Classifier.h"

vector<DSString> parseCSVLine(const DSString& line){
    vector<DSString> tokens;
    string lineStr = line.c_str();
    size_t start = 0;
    size_t pos = lineStr.find(',');

    while(pos != DSString::npos){
        tokens.push_back(line.substring(start, pos - start));
        start = pos + 1;
        pos = lineStr.find(',', start);
    }
    tokens.push_back(line.substring(start, line.length() - start));

    return tokens;
}

vector<DSString> tokenize(const DSString& text){
    vector<DSString> tokens;
    string textStr = text.c_str();
    size_t start = 0;
    size_t pos = textStr.find(' ');

    while(pos != DSString::npos){
        tokens.push_back(text.substring(start, pos - start).toLower());
        start = pos + 1;
        pos = textStr.find(' ', start);
    }
    tokens.push_back(text.substring(start, text.length() - start).toLower());

    return tokens;
}

void trainClassifier(const string& filename, map<DSString, SentimentWordCount>& wordSentiments){
    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Error. Could not open " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        DSString dsLine = line.c_str();
        auto tweetData = parseCSVLine(dsLine);
        int sentiment = stoi(tweetData[0].c_str());
        DSString tweetText = tweetData[5];

        for (auto& word : tokenize(tweetText)) {
            if (sentiment == 4) {
                wordSentiments[word].positiveCount++;
            } 
            else if (sentiment == 0) {
                wordSentiments[word].negativeCount++;
            }
        }
    }
    file.close();
}

int classifyTweet(const DSString& tweet, const map<DSString, SentimentWordCount>& wordSentiments){
    int positiveScore = 0, negativeScore = 0;

    for(const auto& word : tokenize(tweet)){
        if(wordSentiments.count(word)){
            positiveScore += wordSentiments.at(word).positiveCount;
            negativeScore += wordSentiments.at(word).negativeCount;
        }
    }

    return positiveScore >= negativeScore ? 4 : 0;
}

float calculateAccuracy(const string& resultsFileName, const string& groundTruthFilename, const string& accuracyFilename){

}
