#include "icg.h"
#include "optimizer.h"
#include <sstream>
#include <unordered_map>
#include <iostream>

bool Optimizer::isNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void Optimizer::constantFolding(vector<Instruction>& instructions) {
    for (auto& instr : instructions) {
        if (isNumber(instr.arg1) && isNumber(instr.arg2)) {
            int a = stoi(instr.arg1);
            int b = stoi(instr.arg2);
            int res = 0;

            if (instr.op == "+") res = a + b;
            else if (instr.op == "-") res = a - b;
            else if (instr.op == "*") res = a * b;
            else if (instr.op == "/") res = b != 0 ? a / b : 0;
            else continue;

            instr.arg1 = to_string(res);
            instr.op = "";
            instr.arg2 = "";
        }
    }
}

void Optimizer::constantPropagation(vector<Instruction>& instructions) {
    unordered_map<string, string> constants;

    for (auto& instr : instructions) {
        if (instr.op.empty() && isNumber(instr.arg1)) {
            constants[instr.result] = instr.arg1;
        } else {
            if (constants.count(instr.arg1)) instr.arg1 = constants[instr.arg1];
            if (constants.count(instr.arg2)) instr.arg2 = constants[instr.arg2];
        }
    }
}

vector<Instruction> Optimizer::optimize(const vector<Instruction>& icgInstructions) {
    vector<Instruction> optimized = icgInstructions;

    constantFolding(optimized);
    constantPropagation(optimized);

    return optimized;
}
