#include <iostream>
#include <fstream>
#include <map>
#include "csv.hpp"
#include "DSString.h"
#include "Classifier.h"
using namespace std;
using namespace csv;

int main(int argc, char* argv[]) {
    if (argc != 6) { // validates the number of command line arguments given.
        cerr << "Usage: ./sentiment <train_file> <test_file> <ground_truth_file> <results_file> <accuracy_file>" << endl;
        return 1;
    }

    string trainFile = argv[1]; //
    string testFile = argv[2]; //
    string groundTruthFile = argv[3]; // saves the names of the files given through the command line as string variables.
    string resultsFile = argv[4]; //
    string accuracyFile = argv[5]; //

    map<DSString, SentimentWordCount> wordSentiments; // creates a map object using the DSString class and the SentimentWordCount
                                                      // structure.
    trainClassifier(trainFile, wordSentiments); // calls the trainClassifier function using wordSentiments and the training file
                                                // as arguments.

    ifstream testFileStream(testFile); // opens the test file. 
    if (!testFileStream.is_open()) { // validates that testFile is open.
        cerr << "Error: Could not open " << testFile << endl;
        return 1;
    }

    ofstream resultsStream(resultsFile); // opens the results file. 
    if (!resultsStream.is_open()) { // validates that the results file is open.
        cerr << "Error opening results file" << endl;
        return 1;
    }

    CSVReader testReader(testFile); // initializes CSVReader from the CSV Parser library to read the text from the test file.
    int lineCount = 0;
    for (CSVRow& row : testReader) { // for loop used to iterate through every row in the test file.
        lineCount++; // lineCount is increased to indicate that a line has been processed. 
        if (row.size() != 5) { // validates the size of the row. 
            cerr << "Warning: Invalid CSV format in line " << lineCount << ": " << row.to_json() << endl;
            continue;
        }

        try {
            DSString tweetID = DSString(row[0].get<string>().c_str()).trim(); // takes the ID and text of the current tweet, trims
                                                                              // them, and saves them as DSString objects.
            DSString tweetText = DSString(row[4].get<string>().c_str()).trim(); //
            int prediction = classifyTweet(tweetText, wordSentiments); // calls the classifyTweet function to classify the current
                                                                       // tweet's sentiment value and save it as an int variable.
            resultsStream << prediction << ", " << tweetID << endl; // prints the predicted sentiment value and the ID of the
                                                                    // current tweet in the results file.
        } 
        catch (const exception& e) { // outputs an error message if an exception was thrown by the try block.
            cerr << "Error processing test line " << lineCount << ": " << e.what() << endl;
        }
    }

    resultsStream.close(); // closes the results file.

    calculateAccuracy(resultsFile, groundTruthFile, accuracyFile); // calls the calculateAccuracy function to calculate the accuracy
                                                                   // of the program and print it in the accuracy file.

    return 0;
}
