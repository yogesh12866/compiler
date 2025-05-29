// main_syntax.cpp
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <sstream>

using namespace std;

int main() {
    cout << "Enter your source code (end with # on a new line):\n";
    string line, sourceCode;
    while (getline(cin, line)) {
        if (line == "#") break;
        sourceCode += line + "\n";
    }

    // Lexical Analysis
    vector<Token> tokens = tokenize(sourceCode);
    cout << "\n--- Tokens ---\n";
    printTokens(tokens);

    // Syntax Analysis
    cout << "\n--- Syntax Analysis ---\n";
    Parser parser(tokens);
    ParseNode* root = parser.parse();

    cout << "\n--- Parse Tree ---\n";
    parser.printParseTree(root);

    return 0;
}
