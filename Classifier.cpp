#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include "csv.hpp"
#include "Classifier.h"

using namespace std;
using namespace csv;

// function parses a line and splits it into tokens/components in a vector of DSString objects.
vector<DSString> parseCSVLine(const DSString& line) {
    vector<DSString> tokens;
    string lineStr = line.c_str(); // calls the c_str function to convert the DSString object into a C-style string.
    
  
    if (lineStr.empty()) { // validates that lineStr is not empty.
        return tokens;
    }

    stringstream ss(lineStr);
    string token;
    bool insideQuotes = false;
    char c;

    while (ss.get(c)) { // while loop used to iterate through every character in the given string.
        if (c == '"' && !insideQuotes) {
            insideQuotes = true; 
        } 
        else if (c == '"' && insideQuotes) {
            insideQuotes = false; 
        } 
        else if (c == ',' && !insideQuotes) { // if the current character is not inside a quote and it is a comma, any extra 
                                              // whitespaces at the beginning and end of the token are removed. 
  
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);

            tokens.emplace_back(token.c_str()); // the token is added to the tokens vector.
            token.clear(); // the token is reset for the next iteration.
        } 
        else {
            token += c;
        }
    }

    if (!token.empty()) { // checks to see if the last token is empty and removes any extra whitespaces that may have remained. 
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.emplace_back(token.c_str());
    }

    return tokens;
}

// functions splits a given text into separate words/tokens and saves them in lowercase in a vector of DSString objects.
vector<DSString> tokenize(const DSString& text){
    vector<DSString> tokens;
    string textStr = text.c_str(); // calls the c_str function to convert the DSString object into a C-style string.
    size_t start = 0;
    size_t pos = textStr.find(' '); // the end position of the first token is set to the index of the first space.

    while(pos != DSString::npos){ // while loop used to iterate through every word in the given text. 
        tokens.push_back(text.substring(start, pos - start).toLower()); // calls the substring and toLower functions to get a token
                                                                        // from the given text and convert it to lowercase. After
                                                                        // that, it is added to the tokens vector.
        start = pos + 1; // the start position is set to the index after the end of the previous token.
        pos = textStr.find(' ', start); // the new end position is set to the next space.
    }
    tokens.push_back(text.substring(start, text.length() - start).toLower()); // adds the final token to the tokens vector in 
                                                                              // lowercase.

    return tokens;
}

// function reads a file, calls the tokenizer function to tokenize each tweet, and uses each tweet's sentiment value to 
// classify words as positive or negative. 
void trainClassifier(const string& filename, map<DSString, SentimentWordCount>& wordSentiments) {
    CSVReader reader(filename); // initializes CSVReader from the CSV Parser library to read the text from the given filename. 
    int lineCount = 0;

    for (CSVRow& row : reader) { // for loop used to iterate through every row in the file indicated by the given filename.
        lineCount++; // increases lineCount to signify that another row has been processed.
        if (row.size() < 6) { // validates the size of the row. 
            cerr << "Error: Invalid CSV format in line " << lineCount << endl;
            continue;
        }

        try {
            int sentiment = row[0].get<int>(); // saves the sentiment value from the current row as an int variable. 
            DSString tweetText = DSString(row[5].get<>().c_str()).trim(); // saves the tweet's trimmed text as a DSString object.

            for (auto& word : tokenize(tweetText)) { // iterates through the tweet's text and calls the tokenize function to 
                                                     // split the text into separate words. 
                if (sentiment == 4) { // if the sentiment of the row is 4, the current word's positive count is increased. 
                    wordSentiments[word].positiveCount++;
                } 
                else if (sentiment == 0) { // if the sentiment of the row is 0, the current word is associated with negative 
                                           // emotion.
                    wordSentiments[word].negativeCount++;
                }
            }
        } catch (const std::exception& e) { // If there was an error while tokenizing and classifying the sentiment of a row, the
                                            // exception thrown is caught and an error message is output.
            cerr << "Error processing line " << lineCount << ": " << e.what() << endl;
        }
    }
}

// function calls the tokenize function to split the given tweet into separate words and increases either the positive score or
// negative score of the given tweet based on the positiveCount and negativeCount of each word. If the words of a tweet are more
// associated with positivity than negativity, the tweet is classified as 4 for positive. If the words are seen as more negative, 
// the tweet is classified as 0 for negative. 
int classifyTweet(const DSString& tweet, const map<DSString, SentimentWordCount>& wordSentiments){
    int positiveScore = 0, negativeScore = 0;

    for(const auto& word : tokenize(tweet)){ // for loop used to iterate through every word in the tokenized tweet. 
        if(wordSentiments.count(word)){ // if statement that checks to see if the current word exists in the wordSentiments map. If
                                        // it is in the map, the word's positive and negative counts are added to the tweet's 
                                        // overall positive and negative scores.
            positiveScore += wordSentiments.at(word).positiveCount;
            negativeScore += wordSentiments.at(word).negativeCount;
        }
    }

    return positiveScore >= negativeScore ? 4 : 0; // function returns 4 if the positive score is greater than or equal to the  
                                                   // negative score, and it returns 0 if the negative score is greater than the
                                                   // positive score.
}

// function takes the predicted sentiment values from the results file and the actual sentiment values from the ground truth file.
// The function then compares these values, calculates the accuracy of the program, and outputs misclassified tweets to a txt file.
float calculateAccuracy(const string& resultsFileName, const string& groundTruthFilename, const string& accuracyFilename) {
    ifstream resultsFile(resultsFileName); // opens the results file and the ground truth file.
    ifstream groundTruthFile(groundTruthFilename); //

    if (!resultsFile.is_open() || !groundTruthFile.is_open()) { // if either the results file or the ground truth file could not
                                                                // be opened, an error message is output. 
        cerr << "Error. Could not open at least one of the files." << endl;
        return 0.0f;
    }

    string resultsLine, groundTruthLine;
    int totalTweets = 0, correctPredictions = 0;
    int skippedLines = 0;
    bool isFirstLine = true;

    while (getline(resultsFile, resultsLine) && getline(groundTruthFile, groundTruthLine)) { // while loop used to iterate through 
                                                                                             // every line in the results file and
                                                                                             // the ground truth file.
        if (isFirstLine) { // checks if the current line is the first line. If it is the first line, the boolean variable isFirstLine 
                           // is set to false and the loop skips to the next iteration.
            isFirstLine = false;  
            continue;
        }

        DSString dsResultsLine = resultsLine.c_str(); // the c_str function is called to convert the current lines from the results 
                                                      // and the ground truth files into DSString objects.
        DSString dsGroundTruthLine = groundTruthLine.c_str(); //

        auto resultData = parseCSVLine(dsResultsLine); // calls the parseCSVLine function to split up the lines into tokens.
        auto truthData = parseCSVLine(dsGroundTruthLine); //

        if (resultData.size() < 2 || truthData.size() < 1) { // validates the size of the result and truth data.
            skippedLines++;
            continue;
        }

        try {
            int predictedSentiment = stoi(resultData[0].c_str()); // converts the values at the beginning of resultData and truthData
                                                                  // into strings to get the sentiment values. 
            int actualSentiment = stoi(truthData[0].c_str()); //

            if (predictedSentiment == actualSentiment) { // if the predicated sentiment value matches the actual sentiment value,
                                                         // the number of correct predictions is increased. 
                correctPredictions++;
            }
            totalTweets++; // increases totalTweets to signify that another tweet has been processed/compared.
        } 
        catch (const invalid_argument& e) { // catches an exception, prints an error message, and increases the number of skipped
                                            // lines if an incorrect argument was given in the try block.
            cerr << "Error: Invalid argument in line. Error: " << e.what() << endl;
            skippedLines++;
        } 
        catch (const out_of_range& e) { // catches an exception, prints an error message, and increases the number of skipped
                                        // lines if the program went out of range in the try block.
            cerr << "Error: Out of range in line. Error: " << e.what() << endl;
            skippedLines++;
        }
    }

    float accuracy = (static_cast<float>(correctPredictions) / totalTweets) * 100; // calculates the overall accuracy of the 
                                                                                   // program.

    ofstream accuracyFile(accuracyFilename, ios::trunc); // creates/opens the accuracy file in order to output the accuracy results.
    if (!accuracyFile.is_open()) { // validates that the accuracy file is open.
        cerr << "Error. Could not open accuracy file for writing." << endl;
        return accuracy;
    }
    accuracyFile << "Accuracy: " << fixed << setprecision(2) << accuracy << "%\n"; // prints the accuracy of the program rounded to
                                                                                   // two decimal places in the accuracy file.
    accuracyFile.close(); // closes the accuracy file. 

    accuracyFile.open(accuracyFilename, ios::app); // reopens the accuracy file. 
    if (!accuracyFile.is_open()) { // validates that the accuracy file is open.
        cerr << "Error. Could not open accuracy file for appending." << endl;
        return accuracy;
    }

    resultsFile.clear(); //
    resultsFile.seekg(0); // resets and returns to the beginning of the results and ground truth files.
    groundTruthFile.clear(); //
    groundTruthFile.seekg(0); //
    isFirstLine = true;

    while (getline(resultsFile, resultsLine) && getline(groundTruthFile, groundTruthLine)) { // iterates through every line in the 
                                                                                             // results and ground truth files again.
        if (isFirstLine) { // checks if the current line is the first line.
            isFirstLine = false;
            continue;
        }

        DSString dsResultsLine = resultsLine.c_str(); // converts the current lines to DSString objects.
        DSString dsGroundTruthLine = groundTruthLine.c_str(); //

        auto resultData = parseCSVLine(dsResultsLine); // calls the parseCSVLine function to parse the current lines. 
        auto truthData = parseCSVLine(dsGroundTruthLine); // 

        if (resultData.size() < 2 || truthData.size() < 1) { // validates the size of the data. 
            continue;
        }

        try {
            int predictedSentiment = stoi(resultData[0].c_str()); // converts the beginning of resultData and truthData to a string.
            int actualSentiment = stoi(truthData[0].c_str()); //

            if (predictedSentiment != actualSentiment) { // prints the predicted sentiment value, the actual sentiment value, and 
                                                         // the ID of the current tweet to the accuracy file. 
                accuracyFile << predictedSentiment << ", " << actualSentiment
                             << ", " << resultData[1] << endl;
            }
        } 
        catch (const invalid_argument& e) { // skips to the next iteration if an invalid arugment exception was thrown.
            continue;
        } 
        catch (const out_of_range& e) { // skips to the next iteration if an out of range exception was thrown.
            continue;
        }
    }

    if (skippedLines > 0) { // prints out the exact number of skipped lines if any were skipped. 
        cerr << "Skipped " << skippedLines << " lines due to format issues." << endl;
    }

    resultsFile.close(); //
    groundTruthFile.close(); // closes all three files. 
    accuracyFile.close(); //

    return accuracy;
}
