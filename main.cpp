#include <iostream>
#include <fstream>
#include <map>
#include "csv.hpp"
#include "DSString.h"
#include "Classifier.h"
using namespace std;
using namespace csv;

int main(int argc, char* argv[]) {
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
    if (!testFileStream.is_open()) {
        cerr << "Error: Could not open " << testFile << endl;
        return 1;
    }

    ofstream resultsStream(resultsFile);
    if (!resultsStream.is_open()) {
        cerr << "Error opening results file" << endl;
        return 1;
    }

    CSVReader testReader(testFile);
    int lineCount = 0;
    for (CSVRow& row : testReader) {
        lineCount++;
        if (row.size() != 5) {
            cerr << "Warning: Invalid CSV format in line " << lineCount << ": " << row.to_json() << endl;
            continue;
        }

        try {
            DSString tweetID = DSString(row[0].get<string>().c_str()).trim();
            DSString tweetText = DSString(row[4].get<string>().c_str()).trim();
            int prediction = classifyTweet(tweetText, wordSentiments);
            resultsStream << prediction << ", " << tweetID << endl;
        } 
        catch (const exception& e) {
            cerr << "Error processing test line " << lineCount << ": " << e.what() << endl;
        }
    }

    resultsStream.close();

    calculateAccuracy(resultsFile, groundTruthFile, accuracyFile);

    return 0;
}
