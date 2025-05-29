#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "icg.h"
#include <vector>

using namespace std;

class Optimizer {
public:
    vector<Instruction> optimize(const vector<Instruction>& icgInstructions);

private:
    void constantFolding(vector<Instruction>& instructions);
    void constantPropagation(vector<Instruction>& instructions);
    bool isNumber(const string& s);
};

#endif
