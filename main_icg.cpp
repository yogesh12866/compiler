#include "lexer.h"
#include "utils.h"
#include "parser.h"
#include "semantic.h"
#include "icg.h"
#include <iostream>

using namespace std;

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

    if (!root) {
        cout << "\n--- Syntax Error ---\n";
        cout << "Parsing failed. Stopping compilation.\n";
        return 1;
    }

    cout << "\n--- Parse Tree ---\n";
    parser.printParseTree(root);

    SemanticAnalyzer sema;
    sema.analyze(root);
    sema.printErrors();

    if (sema.hasErrors()) {
        cout << "\nCompilation stopped due to semantic errors.\n";
        return 1;
    }

    IntermediateCodeGenerator icg;
    icg.generate(root);
    cout << "\n--- Intermediate Code ---\n";
    icg.printInstructions();

    return 0;
}
