#ifndef CODEGEN_H
#define CODEGEN_H

#include "icg.h"
#include <vector>
#include <string>

class CodeGenerator {
public:
    void generateAssembly(const std::vector<Instruction>& icgInstructions);
};

#endif
