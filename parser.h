#ifndef SYNTAX_H
#define SYNTAX_H

#include <vector>
#include <string>
#include "lexer.h" 

using namespace std;

enum NodeType {
    PROGRAM_NODE,
    STATEMENT_NODE,
    EXPRESSION_NODE,
    IF_STATEMENT_NODE,
    LOOP_STATEMENT_NODE,
    PRINT_STATEMENT_NODE,
    SAN_STATEMENT_NODE,
    DECLARATION_NODE,
    ASSIGNMENT_NODE,
    FUNCTION_CALL_NODE,
    RETURN_STATEMENT_NODE,
    BREAK_STATEMENT_NODE,
    CONTINUE_STATEMENT_NODE,
    OPERATOR_NODE,
    IDENTIFIER_NODE,
    NUMBER_NODE,
    STRING_NODE,
    UNKNOWN_NODE
};

struct ParseNode {
    NodeType type;
    string value;
    vector<ParseNode*> children;
};

class Parser {
private:
    vector<Token> tokens;
    size_t current;

    bool match(TokenType type, string value = "");
    Token peek();
    Token advance();
    bool isAtEnd();
    Token previous();
    void error(const string& message);

    ParseNode* parseProgram();
    ParseNode* parseStmtList();
    ParseNode* parseStmt();
    ParseNode* parseExpr();
    ParseNode* parseTerm();
    ParseNode* parseFactor();
    ParseNode* parseLogic();
    ParseNode* parseComparison();
    ParseNode* parsePrimary();

public:
    Parser(const vector<Token>& tokens);
    ParseNode* parse();
    void printParseTree(ParseNode* node, int level = 0);
    string nodeTypeToString(NodeType type) {
    switch (type) {
        case PROGRAM_NODE: return "PROGRAM_NODE";
        case STATEMENT_NODE: return "STATEMENT_NODE";
        case EXPRESSION_NODE: return "EXPRESSION_NODE";
        case IF_STATEMENT_NODE: return "IF_STATEMENT_NODE";
        case LOOP_STATEMENT_NODE: return "LOOP_STATEMENT_NODE";
        case DECLARATION_NODE: return "DECLARATION_NODE";
        case ASSIGNMENT_NODE: return "ASSIGNMENT_NODE";
        case FUNCTION_CALL_NODE: return "FUNCTION_CALL_NODE";
        case RETURN_STATEMENT_NODE: return "RETURN_STATEMENT_NODE";
        case BREAK_STATEMENT_NODE: return "BREAK_STATEMENT_NODE";
        case CONTINUE_STATEMENT_NODE: return "CONTINUE_STATEMENT_NODE";
        case OPERATOR_NODE: return "OPERATOR_NODE";
        case IDENTIFIER_NODE: return "IDENTIFIER_NODE";
        case NUMBER_NODE: return "NUMBER_NODE";
        case STRING_NODE: return "STRING_NODE";
        case UNKNOWN_NODE: return "UNKNOWN_NODE";
        default: return "UNDEFINED_NODE";
    }
}

};

#endif
