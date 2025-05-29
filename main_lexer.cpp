// main_lexer.cpp
#include "lexer.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    string sourceCode;
    cout << "Enter your source code (end with # on a new line):" << endl;
    string line;
    while (getline(cin, line)) {
        if (line == "#") break;
        sourceCode += line + "\n";
    }

    vector<Token> tokens = tokenize(sourceCode);
    cout << "\n--- Tokens ---\n";
    printTokens(tokens);

    return 0;
}
