#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "icg.h"
#include "optimizer.h"
#include "codegen.h"

class Interpreter {
public:
    void execute(const std::vector<Instruction>& code);

private:
    std::unordered_map<std::string, int> variables;
    std::unordered_map<std::string, int> labels;
    int getValue(const std::string& token);
    bool isNumber(const std::string& s);
};

#endif
