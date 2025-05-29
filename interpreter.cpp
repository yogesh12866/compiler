#include "interpreter.h"
#include <iostream>
#include <cstdlib>

bool Interpreter::isNumber(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s)
        if (!isdigit(c) && c != '-') return false;
    return true;
}

int Interpreter::getValue(const std::string& token) {
    if (isNumber(token)) return std::stoi(token);
    if (variables.count(token)) return variables[token];
    return 0; 
}

void Interpreter::execute(const std::vector<Instruction>& code) {
    std::vector<std::string> paramStack;

    for (int i = 0; i < code.size(); ++i) {
        if (code[i].op == "label") {
            labels[code[i].result] = i;
        }
    }

    for (int pc = 0; pc < code.size(); ++pc) {
        const auto& inst = code[pc];

        if (inst.op == "MOV" || inst.op == "=") {
            variables[inst.result] = getValue(inst.arg1);
        }
        else if (inst.op == "+") {
            variables[inst.result] = getValue(inst.arg1) + getValue(inst.arg2);
        }
        else if (inst.op == "-") {
            variables[inst.result] = getValue(inst.arg1) - getValue(inst.arg2);
        }
        else if (inst.op == "*") {
            variables[inst.result] = getValue(inst.arg1) * getValue(inst.arg2);
        }
        else if (inst.op == "/") {
            int denominator = getValue(inst.arg2);
            variables[inst.result] = (denominator != 0) ? getValue(inst.arg1) / denominator : 0;
        }
        else if (inst.op == "%") {
            int denominator = getValue(inst.arg2);
            variables[inst.result] = (denominator != 0) ? getValue(inst.arg1) % denominator : 0;
        }
        else if (inst.op == ">") {
            variables[inst.result] = getValue(inst.arg1) > getValue(inst.arg2);
        }
        else if (inst.op == "<") {
            variables[inst.result] = getValue(inst.arg1) < getValue(inst.arg2);
        }
        else if (inst.op == ">=") {
            variables[inst.result] = getValue(inst.arg1) >= getValue(inst.arg2);
        }
        else if (inst.op == "<=") {
            variables[inst.result] = getValue(inst.arg1) <= getValue(inst.arg2);
        }
        else if (inst.op == "==") {
            variables[inst.result] = getValue(inst.arg1) == getValue(inst.arg2);
        }
        else if (inst.op == "!=") {
            variables[inst.result] = getValue(inst.arg1) != getValue(inst.arg2);
        }
        else if (inst.op == "ifFalse") {
            if (!getValue(inst.arg1)) {
                if (labels.count(inst.result))
                    pc = labels[inst.result] - 1;
            }
        }
        else if (inst.op == "goto") {
            if (labels.count(inst.result))
                pc = labels[inst.result] - 1;
        }
        else if (inst.op == "param") {
            if (!inst.arg1.empty())
                paramStack.push_back(inst.arg1);
            else
                paramStack.push_back(inst.result);
        }
        else if (inst.op == "call") {
            if (inst.result == "prrint" && !paramStack.empty()) {
                std::cout << getValue(paramStack.back()) << std::endl;
                paramStack.clear();
            }
        }
        else if (inst.op == "print") {
    if (variables.find(inst.arg1) != variables.end())
        cout << variables[inst.arg1] << endl;
    else
        cout << inst.arg1 << endl;  
}


    }
}
