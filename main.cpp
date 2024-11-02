#include <iostream>
#include <fstream>
#include <map>
#include "DSString.h"
#include "Classifier.h"
using namespace std;

int main(int argc, char* argv[]){
    if (argc != 6) {
        cerr << "Usage: ./sentiment <train_file> <test_file> <ground_truth_file> <results_file> <accuracy_file>" << endl;
        return 1;
    }

    string trainFile = argv[1];
    string testFile = argv[2];
    string groundTruthFile = argv[3];
    string resultsFile = argv[4];
    string accuracyFile = argv[5];

    map<DSString, SentimentWordCount> wordSentiments;
    trainClassifier(trainFile, wordSentiments);

    ifstream testFileStream(testFile);
    ofstream resultsStream(resultsFile);
    if (!testFileStream.is_open() || !resultsStream.is_open()) {
        cerr << "Error opening test file or results file" << endl;
        return 1;
    }

    string testLine;
    while (getline(testFileStream, testLine)) {
        DSString dsTestLine = testLine.c_str();
        auto tweetData = parseCSVLine(dsTestLine);
        DSString tweetText = tweetData[5];
        int prediction = classifyTweet(tweetText, wordSentiments);
        resultsStream << prediction << ", " << tweetData[1] << endl;
    }

    testFileStream.close();
    resultsStream.close();

    calculateAccuracy(resultsFile, groundTruthFile, accuracyFile);

    return 0;
}
