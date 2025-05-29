#include "semantic.h"
#include <iostream>
#include <regex>
using namespace std;

void SemanticAnalyzer::analyze(ParseNode* root) {
    symbolTable.clear();
    errors.clear();
    loopDepth = 0;
    currentReturnType = "intt"; 
    traverse(root);
}

void SemanticAnalyzer::traverse(ParseNode* node) {
    if (!node) return;

    switch (node->type) {
        case DECLARATION_NODE: {
            size_t spacePos = node->value.find(' ');
            string varType = node->value.substr(0, spacePos);
            string varName = node->value.substr(spacePos + 1);

            if (symbolTable.count(varName)) {
                errors.push_back("Error: Redeclaration of variable '" + varName + "'");
            } else {
                symbolTable[varName] = {varType, varName};
            }

            
            if (!node->children.empty()) {
                string exprType = getExprType(node->children[0]);
                if (exprType != varType) {
                    errors.push_back("Type Error: Cannot assign type '" + exprType + "' to variable '" + varName + "' of type '" + varType + "'");
                }
                traverse(node->children[0]);
            }
            break;
        }

        case ASSIGNMENT_NODE: {
            if (!symbolTable.count(node->value)) {
                errors.push_back("Error: Assignment to undeclared variable '" + node->value + "'");
            } else {
                string varType = symbolTable[node->value].type;
                if (!node->children.empty()) {
                    string exprType = getExprType(node->children[0]);
                    if (exprType != varType) {
                        errors.push_back("Type Error: Cannot assign type '" + exprType + "' to variable '" + node->value + "' of type '" + varType + "'");
                    }
                    traverse(node->children[0]);
                }
            }
            break;
        }

        case IDENTIFIER_NODE: {
            if (!symbolTable.count(node->value)) {
                errors.push_back("Error: Undeclared variable '" + node->value + "'");
            }
            break;
        }

        case RETURN_STATEMENT_NODE: {
            if (!node->children.empty()) {
                string retType = getExprType(node->children[0]);
                if (retType != currentReturnType) {
                    errors.push_back("Type Error: Return type '" + retType + "' does not match function return type '" + currentReturnType + "'");
                }
                traverse(node->children[0]);
            }
            break;
        }

        case FUNCTION_CALL_NODE: {
            // Built-in function calls: prrint and san
            string funcName = node->value;
            if (funcName == "prrint") {
                if (node->children.size() != 1) {
                    errors.push_back("Error: 'prrint' expects exactly one argument");
                } else {
                    // Just allow intt or sttring types for now
                    string argType = getExprType(node->children[0]);
                    if (argType != "intt" && argType != "sttring") {
                        errors.push_back("Type Error: 'prrint' argument must be intt or sttring, got '" + argType + "'");
                    }
                    traverse(node->children[0]);
                }
            }
            else if (funcName == "san") {
    if (node->children.size() != 1) {
        errors.push_back("Error: 'san' expects exactly one argument");
    } else {
        string argType = getExprType(node->children[0]);
        if (argType != "intt") {
            errors.push_back("Type Error: 'san' argument must be of type intt, got '" + argType + "'");
        }
        traverse(node->children[0]);
    }
}
else {
                errors.push_back("Error: Unknown function '" + funcName + "'");
            }
            break;
        }

        case LOOP_STATEMENT_NODE: {
            loopDepth++;
            for (auto* child : node->children) traverse(child);
            loopDepth--;
            break;
        }

        case BREAK_STATEMENT_NODE:
        case CONTINUE_STATEMENT_NODE: {
            if (loopDepth == 0) {
                errors.push_back("Error: '" + node->value + "' used outside of loop");
            }
            break;
        }

        default: {
            for (auto* child : node->children) traverse(child);
            break;
        }
    }
}

string SemanticAnalyzer::getExprType(ParseNode* node) {
    if (!node) return "unknown";

    switch (node->type) {
        case NUMBER_NODE:
            return "intt";  // Assuming all numbers are intt for now

        case STRING_NODE:
            return "sttring";

        case IDENTIFIER_NODE: {
            if (symbolTable.count(node->value)) {
                return symbolTable[node->value].type;
            } else {
                errors.push_back("Error: Undeclared variable '" + node->value + "'");
                return "unknown";
            }
        }

        case EXPRESSION_NODE: {
    if (node->value.size() >= 2 && node->value.front() == '"' && node->value.back() == '"') {
        return "sttring";
    }

    if (regex_match(node->value, regex("^[0-9]+$"))) {
        return "intt";
    }

    if (node->value == "+" || node->value == "-" || node->value == "*" || node->value == "/" ||
        node->value == "==" || node->value == "!=" || node->value == "<" || node->value == "<=" ||
        node->value == ">" || node->value == ">=" || node->value == "&&" || node->value == "||") {

        if (node->children.size() < 2) return "unknown";

        string leftType = getExprType(node->children[0]);
        string rightType = getExprType(node->children[1]);

        if (node->value == "&&" || node->value == "||") {
            return "intt";  
        }

        if (leftType != rightType) {
            errors.push_back("Type Error: Mismatched types '" + leftType + "' and '" + rightType + "' in operation '" + node->value + "'");
            return "unknown";
        }

        if (node->value == "==" || node->value == "!=" ||
            node->value == "<" || node->value == "<=" ||
            node->value == ">" || node->value == ">=") {
            return "intt";
        }

        return leftType; 
    }

    
    if (symbolTable.count(node->value)) {
        return symbolTable[node->value].type;
    }

    return "unknown"; 
}

        default:
            return "unknown";
    }
}

void SemanticAnalyzer::printErrors() {
    cout << "\n--- Semantic Analysis ---\n";
    if (errors.empty()) {
        cout << "No semantic errors.\n";
    } else {
        for (const string& err : errors) {
            cout << err << endl;
        }
    }
}

bool SemanticAnalyzer::hasErrors() const {
    return !errors.empty();
}
