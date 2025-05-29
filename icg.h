#ifndef ICG_H
#define ICG_H

#include <string>
#include <vector>
#include "parser.h"   
#include "lexer.h" 

using namespace std;

struct Instruction {
    string op;
    string arg1;
    string arg2;
    string result;
};

class IntermediateCodeGenerator {
private:
    int tempCount;  
    int labelCount; 

    
    vector<pair<string, string>> labelStack;

    string newTemp();
    string newLabel();
    string evaluateExpression(ParseNode* node);  

    void traverse(ParseNode* node);  

public:
    vector<Instruction> instructions;
    IntermediateCodeGenerator();
    void generate(ParseNode* root);  
    void printInstructions();   
    vector<Instruction> getICG();   

};

#endif
