//g++ -std=gnu++17 executable.cpp lexer.cpp parser.cpp semantic.cpp icg.cpp optimizer.cpp codegen.cpp interpreter.cpp -o executable.exe

// .\executable.exe

#include <iostream>
#include <string>
#include <vector>
#include "interpreter.h" 
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "icg.h"
#include "optimizer.h"
#include "codegen.h"
#include "interpreter.h"
using namespace std;

int main() {
    cout << "Enter your source code (end with # on a new line):\n";
    string line, code;
    while (getline(cin, line)) {
        if (line == "#") break;
        code += line + "\n";
    }

    // --- Lexical Analysis ---
    vector<Token> tokens = tokenize(code);
    cout << "\n--- Tokens ---\n";
    printTokens(tokens);

    // --- Syntax Analysis ---
    Parser parser(tokens);
    ParseNode* root = parser.parse();

    if (!root) {
        cout << "\n--- Syntax Error ---\n";
        return 1;
    }

    cout << "\n--- Parse Tree ---\n";
    parser.printParseTree(root);

    // --- Semantic Analysis ---
    SemanticAnalyzer sema;
    sema.analyze(root);
    sema.printErrors();

    if (sema.hasErrors()) {
        cout << "\nCompilation stopped due to semantic errors.\n";
        return 1;
    }

    // --- Intermediate Code Generation ---
    IntermediateCodeGenerator icg;
    icg.generate(root);

    cout << "\n--- Intermediate Code ---\n";
    icg.printInstructions();

    // --- Optimization ---
    Optimizer optimizer;
    vector<Instruction> optimized = optimizer.optimize(icg.getICG());

    cout << "\n--- Optimized Code ---\n";
    for (auto& instr : optimized) {
        if (instr.op == "label") {
            cout << instr.result << ":\n";
        } else {
            cout << instr.op << " " << instr.arg1;
            if (!instr.arg2.empty()) cout << ", " << instr.arg2;
            if (!instr.result.empty()) cout << " => " << instr.result;
            cout << endl;
        }
    }

    // --- Code Generation (Assembly stub) ---
    CodeGenerator codegen;
    codegen.generateAssembly(optimized);  

    // --- Execution ---
    Interpreter interpreter;
    cout << "\n--- Output ---\n";
    interpreter.execute(optimized);

    return 0;
}
