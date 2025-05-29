#include "codegen.h"
#include <iostream>

using namespace std;

void CodeGenerator::generateAssembly(const vector<Instruction>& icgInstructions) {
    int regCount = 0;

    for (const auto& instr : icgInstructions) {
        if (instr.op.empty()) {
            // Simple assignment
            cout << "MOV " << instr.result << ", " << instr.arg1 << endl;
        } else {
            // Binary operation
            string reg = "R" + to_string(regCount++);
            cout << "MOV " << reg << ", " << instr.arg1 << endl;
            cout << instr.op << " " << reg << ", " << instr.arg2 << endl;
            cout << "MOV " << instr.result << ", " << reg << endl;
        }
    }
}
