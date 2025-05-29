#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "icg.h"
#include "optimizer.h"
#include <vector>
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

    Optimizer optimizer;
    vector<Instruction> optimized = optimizer.optimize(icg.getICG());

    cout << "\n--- Optimized Code ---\n";
    for (const auto& instr : optimized) {
        if (instr.op == "label") {
            cout << instr.result << ":\n";
        } else if (instr.op == "goto") {
            cout << "goto " << instr.result << "\n";
        } else if (instr.op == "ifFalse") {
            cout << "ifFalse " << instr.arg1 << " goto " << instr.result << "\n";
        } else if (instr.op == "call") {
            cout << "call " << instr.result << "\n";
        } else if (instr.op == "return") {
            cout << "return " << instr.arg1 << "\n";
        } else if (instr.op == "=") {
            cout << instr.result << " = " << instr.arg1 << "\n";
        } else if (instr.op == "param") {
            cout << "param " << instr.arg1 << "\n";
        } else if (instr.op == "print") {
            cout << "print " << instr.arg1 << "\n";
        } else {
            cout << instr.result << " = " << instr.arg1 << " " << instr.op << " " << instr.arg2 << "\n";
        }
    }

    return 0;
}
