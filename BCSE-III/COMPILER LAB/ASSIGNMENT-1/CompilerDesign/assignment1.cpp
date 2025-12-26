#include<iostream>
#include<fstream>
#include<string>
using namespace std;

// Constants for file names
const string keywordsFile = "keywords.txt";
const string punctuationsFile = "punctuations.txt";
const string operatorsFile = "operators.txt";
const string inputFile = "targetc.c";
const string symbolTableFile = "symboltable.txt";

// Helper functions to retrieve data from files and store them as arrays or hash maps

void buildKeywordSet(bool keywordSet[256]) {
    ifstream fi(keywordsFile);
    string word;
    while (fi >> word) {
        keywordSet[word[0]] = true;
    }
    fi.close();
}

void buildPunctuationSet(bool punctuationSet[256]) {
    ifstream fi(punctuationsFile);
    char ch;
    while (fi >> ch) {
        punctuationSet[ch] = true;
    }
    fi.close();
}

void buildOperatorSet(bool operatorSet[256]) {
    ifstream fi(operatorsFile);
    char ch;
    while (fi >> ch) {
        operatorSet[ch] = true;
    }
    fi.close();
}

// Helper function to check if the string is a valid keyword or identifier
bool isKeyword(const string &token, bool keywordSet[256]) {
    return keywordSet[token[0]];
}

void computeTokens() {
    ifstream fi(inputFile);
    string str;
    int row = 1;
    bool keywordSet[256] = {false}; // Array to track keyword characters
    bool punctuationSet[256] = {false}; // Array to track punctuation characters
    bool operatorSet[256] = {false}; // Array to track operator characters

    // Build the sets from files
    buildKeywordSet(keywordSet);
    buildPunctuationSet(punctuationSet);
    buildOperatorSet(operatorSet);

    // Output header for token table
    cout << "----------------------------------------------------------------------\n";
    printf("\tToken \t\tType \t\tRow \t\tColumn \n");
    cout << "----------------------------------------------------------------------\n";

    // Process each line from the input file
    while (getline(fi, str)) {
        string token;
        int startidx = -1, endidx = -1;
        for (int i = 0; i <= str.size(); ++i) {
            // Tokenize the string based on spaces or end of line
            if (i == str.size() || str[i] == ' ') {
                if (startidx != -1) {
                    token = str.substr(startidx, endidx - startidx + 1);
                    int col = startidx + 1;
                    if (isKeyword(token, keywordSet)) {
                        cout << "\t" << token << "\t\tkeyword \t" << row << "\t\t" << col << "\n";
                    } else {
                        cout << "\t" << token << "\t\tidentifier \t" << row << "\t\t" << col << "\n";
                    }
                }
                startidx = -1;
            }
            // Check for compound operators
            else if (i + 1 < str.size() &&
                     ((str[i] == '+' && str[i + 1] == '+') || 
                      (str[i] == '-' && str[i + 1] == '-') || 
                      (str[i] == '+' && str[i + 1] == '=') || 
                      (str[i] == '-' && str[i + 1] == '=') || 
                      (str[i] == '*' && str[i + 1] == '=') || 
                      (str[i] == '/' && str[i + 1] == '='))) {
                if (startidx != -1) {
                    token = str.substr(startidx, endidx - startidx + 1);
                    int col = startidx + 1;
                    cout << "\t" << token << "\t\tidentifier \t" << row << "\t\t" << col << "\n";
                }
                token = str.substr(i, 2);
                int col = i + 1;
                cout << "\t" << token << "\t\toperator \t" << row << "\t\t" << col << "\n";
                i++; // Skip the next character since it's part of the compound operator
                startidx = -1;
            }
            else if (operatorSet[str[i]]) {
                if (startidx != -1) {
                    token = str.substr(startidx, endidx - startidx + 1);
                    int col = startidx + 1;
                    cout << "\t" << token << "\t\tidentifier \t" << row << "\t\t" << col << "\n";
                }
                token = str[i];
                int col = i + 1;
                cout << "\t" << token << "\t\toperator \t" << row << "\t\t" << col << "\n";
                startidx = -1;
            } 
            else if (punctuationSet[str[i]]) {
                if (startidx != -1) {
                    token = str.substr(startidx, endidx - startidx + 1);
                    int col = startidx + 1;
                    cout << "\t" << token << "\t\tidentifier \t" << row << "\t\t" << col << "\n";
                }
                token = str[i];
                int col = i + 1;
                cout << "\t" << token << "\t\tpunctuation \t" << row << "\t\t" << col << "\n";
                startidx = -1;
            } else if (startidx == -1) {
                startidx = endidx = i;
            } else {
                endidx = i;
            }
        }
        row++;
    }
    cout << "----------------------------------------------------------------------\n";
    fi.close();
}


void generateSymbolTableFile() {
    ofstream fo(symbolTableFile, ios::out | ios::trunc);
    if (fo.is_open()) {
        fo << "Symbol Table Content: \n";
        // Write symbols to the file here...
        // This section assumes symbols are stored in an array/list
        // and symbols are collected during the tokenization process.
    }
    fo.close();
}

int main() {
    // Initialize arrays for sets
    bool keywordSet[256] = {false};
    bool punctuationSet[256] = {false};
    bool operatorSet[256] = {false};

    // Build sets for keywords, punctuation, and operators
    buildKeywordSet(keywordSet);
    buildPunctuationSet(punctuationSet);
    buildOperatorSet(operatorSet);

    // Process the input file and generate tokens
    computeTokens();

    // Generate symbol table
    generateSymbolTableFile();

    return 0;
}

