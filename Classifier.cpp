#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include "csv.hpp"
#include "Classifier.h"

using namespace std;
using namespace csv;

vector<DSString> parseCSVLine(const DSString& line) {
    vector<DSString> tokens;
    string lineStr = line.c_str();
    
  
    if (lineStr.empty()) {
        return tokens;
    }

    stringstream ss(lineStr);
    string token;
    bool insideQuotes = false;
    char c;

    while (ss.get(c)) {
        if (c == '"' && !insideQuotes) {
            insideQuotes = true; 
        } else if (c == '"' && insideQuotes) {
            insideQuotes = false; 
        } else if (c == ',' && !insideQuotes) {
  
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);

            tokens.emplace_back(token.c_str());
            token.clear();
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.emplace_back(token.c_str());
    }

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

void trainClassifier(const string& filename, map<DSString, SentimentWordCount>& wordSentiments) {
    CSVReader reader(filename);
    int lineCount = 0;

    for (CSVRow& row : reader) {
        lineCount++;
        if (row.size() < 6) {
            cerr << "Error: Invalid CSV format in line " << lineCount << endl;
            continue;
        }

        try {
            int sentiment = row[0].get<int>();
            DSString tweetText = DSString(row[5].get<>().c_str()).trim();

            for (auto& word : tokenize(tweetText)) {
                if (sentiment == 4) {
                    wordSentiments[word].positiveCount++;
                } else if (sentiment == 0) {
                    wordSentiments[word].negativeCount++;
                }
            }
        } catch (const std::exception& e) {
            cerr << "Error processing line " << lineCount << ": " << e.what() << endl;
        }
    }
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

float calculateAccuracy(const string& resultsFileName, const string& groundTruthFilename, const string& accuracyFilename) {
    ifstream resultsFile(resultsFileName);
    ifstream groundTruthFile(groundTruthFilename);
    ofstream accuracyFile(accuracyFilename);

    if (!resultsFile.is_open() || !groundTruthFile.is_open() || !accuracyFile.is_open()) {
        cerr << "Error. Could not open at least one of the files." << endl;
        return 0.0f;
    }

    string resultsLine, groundTruthLine;
    int totalTweets = 0, correctPredictions = 0;
    int skippedLines = 0;
    bool isFirstLine = true;

    while (getline(resultsFile, resultsLine) && getline(groundTruthFile, groundTruthLine)) {
        if (isFirstLine) {
            isFirstLine = false;  
            continue;
        }

        DSString dsResultsLine = resultsLine.c_str();
        DSString dsGroundTruthLine = groundTruthLine.c_str();

        auto resultData = parseCSVLine(dsResultsLine);
        auto truthData = parseCSVLine(dsGroundTruthLine);
        
        if (resultData.size() < 3 || truthData.size() < 2) {
            skippedLines++;
            continue;
        }

        try {
            if (!resultData[0].empty() && !truthData[0].empty() && 
                isdigit(resultData[0][0]) && isdigit(truthData[0][0])) {
                
                int predictedSentiment = stoi(resultData[0].c_str());
                int actualSentiment = stoi(truthData[0].c_str());

                if (predictedSentiment == actualSentiment) {
                    correctPredictions++;
                } else {
                    accuracyFile << predictedSentiment << ", " << actualSentiment 
                                 << ", " << resultData[1] << endl;
                }
                totalTweets++;
            } else {
                skippedLines++;
            }
        } catch (const invalid_argument& e) {
            cerr << "Error: Invalid argument in line. Error: " << e.what() << endl;
            skippedLines++;
        } catch (const out_of_range& e) {
            cerr << "Error: Out of range in line. Error: " << e.what() << endl;
            skippedLines++;
        }
    }

    float accuracy = (static_cast<float>(correctPredictions) / totalTweets) * 100;
    accuracyFile << "Accuracy: " << fixed << setprecision(2) << accuracy << "%\n";

    if (skippedLines > 0) {
        cerr << "Skipped " << skippedLines << " lines due to format issues." << endl;
    }

    resultsFile.close();
    groundTruthFile.close();
    accuracyFile.close();

    return accuracy;
}
