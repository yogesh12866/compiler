// main_semantic.cpp
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"

int main() {
    cout << "Enter your source code (end with # on a new line):\n";
    string line, code;
    while (getline(cin, line)) {
        if (line == "#") break;
        code += line + "\n";
    }

    vector<Token> tokens = tokenize(code);
    cout << "\n--- Tokens ---\n";
    printTokens(tokens);

    Parser parser(tokens);
    ParseNode* root = parser.parse();

    if (root) {
        cout << "\n--- Parse Tree ---\n";
        parser.printParseTree(root);
        SemanticAnalyzer sema;
        sema.analyze(root);
        sema.printErrors();
    }

    return 0;
}
