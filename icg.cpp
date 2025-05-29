#include "icg.h"
#include <iostream>
using namespace std;

IntermediateCodeGenerator::IntermediateCodeGenerator() {
    tempCount = 0;
    labelCount = 0;
}

vector<Instruction> IntermediateCodeGenerator::getICG() {
    return instructions;
}

string IntermediateCodeGenerator::newTemp() {
    return "t" + to_string(tempCount++);
}

string IntermediateCodeGenerator::newLabel() {
    return "L" + to_string(labelCount++);
}

string IntermediateCodeGenerator::evaluateExpression(ParseNode* node) {
    if (!node) return "";

    // Leaf nodes: identifiers, numbers, strings
    if (node->type == IDENTIFIER_NODE || node->type == NUMBER_NODE || node->type == STRING_NODE) {
        return node->value;
    }

    // Function call nodes
    if (node->type == FUNCTION_CALL_NODE) {
    string arg = "";
    if (!node->children.empty()) {
        arg = evaluateExpression(node->children[0]);
        instructions.push_back({"param", arg, "", ""});
    }

    // ADD THIS BLOCK:
    if (node->value == "prrint" || node->value == "san") {
        instructions.push_back({"print", arg, "", ""});
        return "";
    }

    instructions.push_back({"call", "", "", node->value});
    return "";
}


    // Binary expressions
    if (node->type == EXPRESSION_NODE) {
        if (node->children.size() >= 3) {
            string acc = evaluateExpression(node->children[0]);
            for (size_t i = 2; i < node->children.size(); i += 2) {
                string op = node->children[i - 1]->value;
                string next = evaluateExpression(node->children[i]);
                string temp = newTemp();
                instructions.push_back({op, acc, next, temp});
                acc = temp;
            }
            return acc;
        } else if (node->children.size() == 2) {
            string left = evaluateExpression(node->children[0]);
            string right = evaluateExpression(node->children[1]);
            string op = node->value.empty() ? "+" : node->value;
            string temp = newTemp();
            instructions.push_back({op, left, right, temp});
            return temp;
        } else if (!node->children.empty()) {
            return evaluateExpression(node->children[0]);
        }
    }

    return "";
}

void IntermediateCodeGenerator::traverse(ParseNode* node) {
    if (!node) return;

    switch (node->type) {
        case PROGRAM_NODE:
            for (auto child : node->children) {
                traverse(child);
            }
            break;

        case STATEMENT_NODE:  
            for (auto child : node->children) {
                traverse(child);
            }
            break;

        case DECLARATION_NODE: {
            string id = "";
            string exprResult = "";
            for (auto child : node->children) {
                if (child->type == IDENTIFIER_NODE) {
                    id = child->value;
                } else {
                    exprResult = evaluateExpression(child);
                }
            }

            if (id.empty()) {
                size_t spacePos = node->value.find(' ');
                if (spacePos != string::npos) {
                    id = node->value.substr(spacePos + 1);
                }
            }

            if (!id.empty() && !exprResult.empty()) {
                instructions.push_back({"=", exprResult, "", id});
            }
            break;
        }

        case ASSIGNMENT_NODE: {
    string id = node->value;
    string expr = evaluateExpression(node->children[0]);
    if (!expr.empty())
        instructions.push_back({"=", expr, "", id});
    break;
}


        case IF_STATEMENT_NODE: {
            string elseLabel = newLabel();
            string endLabel = newLabel();
            string cond = evaluateExpression(node->children[0]);

            instructions.push_back({"ifFalse", cond, "", elseLabel});
            traverse(node->children[1]); 

            instructions.push_back({"goto", "", "", endLabel});
            instructions.push_back({"label", "", "", elseLabel});

            if (node->children.size() == 3) {
                traverse(node->children[2]); 
            }

            instructions.push_back({"label", "", "", endLabel});
            break;
        }

        case LOOP_STATEMENT_NODE: {
            string startLabel = newLabel();
            string endLabel = newLabel();
            labelStack.push_back({startLabel, endLabel});

            instructions.push_back({"label", "", "", startLabel});
            string cond = evaluateExpression(node->children[0]);
            instructions.push_back({"ifFalse", cond, "", endLabel});

            traverse(node->children[1]);

            instructions.push_back({"goto", "", "", startLabel});
            instructions.push_back({"label", "", "", endLabel});

            labelStack.pop_back();
            break;
        }

        case BREAK_STATEMENT_NODE:
            if (!labelStack.empty()) {
                instructions.push_back({"goto", "", "", labelStack.back().second});
            }
            break;

        case CONTINUE_STATEMENT_NODE:
            if (!labelStack.empty()) {
                instructions.push_back({"goto", "", "", labelStack.back().first});
            }
            break;

        case RETURN_STATEMENT_NODE: {
            string retVal = "";
            if (!node->children.empty()) {
                retVal = evaluateExpression(node->children[0]);
            }
            instructions.push_back({"return", retVal, "", ""});
            break;
        }

        case FUNCTION_CALL_NODE: {
    string arg = "";
    if (!node->children.empty()) {
        arg = evaluateExpression(node->children[0]);
    }

    // Generate actual instruction for prrint/san
    if (node->value == "prrint" || node->value == "san") {
        instructions.push_back({"print", arg, "", ""});
    } else {
        // generic function call
        instructions.push_back({"param", arg, "", ""});
        instructions.push_back({"call", "", "", node->value});
    }
    break;
}


        case PRINT_STATEMENT_NODE: {
            string toPrint;
            if (!node->children.empty()) {
                toPrint = evaluateExpression(node->children[0]);
            }
            instructions.push_back({"print", toPrint, "", ""});
            break;
        }

        case SAN_STATEMENT_NODE:
            instructions.push_back({"print", "\"SAN\"", "", ""});
            break;

        default:
            for (auto child : node->children) {
                traverse(child);
            }
            break;
    }
}

void IntermediateCodeGenerator::generate(ParseNode* root) {
    traverse(root);
}

void IntermediateCodeGenerator::printInstructions() {
    for (const auto& instr : instructions) {
        if (instr.op == "label") {
            cout << instr.result << ":" << endl;
        } else if (instr.op == "goto") {
            cout << "goto " << instr.result << endl;
        } else if (instr.op == "ifFalse") {
            cout << "ifFalse " << instr.arg1 << " goto " << instr.result << endl;
        } else if (instr.op == "call") {
            cout << "call " << instr.result << endl;
        } else if (instr.op == "return") {
            cout << "return " << instr.arg1 << endl;
        } else if (instr.op == "=") {
            cout << instr.result << " = " << instr.arg1 << endl;
        } else if (instr.op == "param") {
            cout << "param " << instr.arg1 << endl;
        } else if (instr.op == "print") {
            cout << "print " << instr.arg1 << endl;
        } else {
            cout << instr.result << " = " << instr.arg1 << " " << instr.op << " " << instr.arg2 << endl;
        }
    }
}

