#include "parser.h"
#include <iostream>

using namespace std;

Parser::Parser(const vector<Token>& tokens) : tokens(tokens), current(0) {}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::isAtEnd() {
    return current >= tokens.size();
}

bool Parser::match(TokenType type, string value) {
    if (isAtEnd()) return false;
    Token t = peek();
    if (t.type == type && (value.empty() || t.value == value)) {
        advance();
        return true;
    }
    return false;
}

void Parser::error(const string& message) {
    string err = "Syntax Error: " + message + " at token: '" + peek().value + "'";
    cerr << err << endl << flush;
    cout << err << endl; 
    exit(1);
}


// ---- Recursive Descent Parsing ----

ParseNode* Parser::parse() {
    return parseProgram();
}

ParseNode* Parser::parseProgram() {
    if (match(KEYWORD, "intt") || match(KEYWORD, "sttring")) {
        // optional return type
    }

    if (!match(KEYWORD, "mainn")) error("Expected 'mainn'");
    if (!match(DELIMITER, "(")) error("Expected '(' after 'mainn'");
    if (!match(DELIMITER, ")")) error("Expected ')' after '('");
    if (!match(DELIMITER, "{")) error("Expected '{' after mainn()");

    ParseNode* node = new ParseNode{PROGRAM_NODE, "mainn", {}};

    if (!isAtEnd() && peek().value != "}") {
        node->children.push_back(parseStmtList());
    }

    if (!match(DELIMITER, "}")) {
        error("Expected '}' at end of mainn");
    }

    return node;
}


ParseNode* Parser::parseStmtList() {
    ParseNode* node = new ParseNode{STATEMENT_NODE, "stmt_list", {}};
    while (!isAtEnd() && peek().value != "}") {
        node->children.push_back(parseStmt());
    }
    return node;
}

ParseNode* Parser::parseStmt() {
    if (match(KEYWORD, "intt") || match(KEYWORD, "sttring")) {
        string type = previous().value;
        if (!match(IDENTIFIER)) error("Expected identifier after type");
        Token id = previous();
        ParseNode* decl = new ParseNode{DECLARATION_NODE, type + " " + id.value, {}};
        if (match(OPERATOR, "=")) {
            decl->children.push_back(parseExpr());
        }
        if (!match(DELIMITER, ";")) error("Expected ';' after declaration");
        return decl;
    }

    if (match(IDENTIFIER)) {
        Token id = previous();
        if (match(OPERATOR, "=")) {
            ParseNode* rhs = parseExpr();
            ParseNode* assign = new ParseNode{ASSIGNMENT_NODE, id.value, {}};
            assign->children.push_back(rhs);

            if (!match(DELIMITER, ";")) error("Expected ';' after assignment");
            return assign;
        } else {
            error("Expected '=' after identifier");
        }
    }


    if (match(KEYWORD, "retturn")) {
        ParseNode* ret = new ParseNode{RETURN_STATEMENT_NODE, "retturn", {}};
        if (peek().value != ";") {
            ret->children.push_back(parseExpr());
        }
        if (!match(DELIMITER, ";")) error("Expected ';' after retturn");
        return ret;
    }

    if (match(KEYWORD, "prrint") || match(KEYWORD, "san")) {
        Token func = previous();
        if (!match(DELIMITER, "(")) error("Expected '(' after function name");
        ParseNode* call = new ParseNode{FUNCTION_CALL_NODE, func.value, {}};
        if (peek().type != DELIMITER || peek().value != ")") {
            call->children.push_back(parseExpr());
        }
        if (!match(DELIMITER, ")")) error("Expected ')' after args");
        if (!match(DELIMITER, ";")) error("Expected ';' after function call");
        return call;
    }

    if (match(KEYWORD, "iif")) {
        ParseNode* ifNode = new ParseNode{IF_STATEMENT_NODE, "iif", {}};
        if (!match(DELIMITER, "(")) error("Expected '(' after iif");
        ifNode->children.push_back(parseExpr());
        if (!match(DELIMITER, ")")) error("Expected ')' after condition");
        if (!match(DELIMITER, "{")) error("Expected '{' after iif()");
        ifNode->children.push_back(parseStmtList());
        if (!match(DELIMITER, "}")) error("Expected '}' after if body");
        if (match(KEYWORD, "ellse")) {
            if (!match(DELIMITER, "{")) error("Expected '{' after ellse");
            ifNode->children.push_back(parseStmtList());
            if (!match(DELIMITER, "}")) error("Expected '}' after else body");
        }
        return ifNode;
    }

    if (match(KEYWORD, "loop")) {
        ParseNode* loopNode = new ParseNode{LOOP_STATEMENT_NODE, "loop", {}};
        if (!match(DELIMITER, "(")) error("Expected '(' after 'loop'");
        loopNode->children.push_back(parseExpr());
        if (!match(DELIMITER, ")")) error("Expected ')' after loop condition");
        if (!match(DELIMITER, "{")) error("Expected '{' after loop condition");
        loopNode->children.push_back(parseStmtList());
        if (!match(DELIMITER, "}")) error("Expected '}' after loop body");
        return loopNode;
    }

    if (match(KEYWORD, "brreak")) {
        ParseNode* breakNode = new ParseNode{BREAK_STATEMENT_NODE, "brreak", {}};
        if (!match(DELIMITER, ";")) error("Expected ';' after 'brreak'");
        return breakNode;
    }

    if (match(KEYWORD, "conttinue")) {
        ParseNode* continueNode = new ParseNode{CONTINUE_STATEMENT_NODE, "conttinue", {}};
        if (!match(DELIMITER, ";")) error("Expected ';' after 'conttinue'");
        return continueNode;
    }

    error("Unknown or invalid statement");
    return nullptr;
}


// --- Expression Parsing ---

ParseNode* Parser::parseExpr() {
    return parseLogic();  
}

ParseNode* Parser::parseLogic() {
    ParseNode* node = parseComparison();
    while (match(OPERATOR, "&&") || match(OPERATOR, "||")) {
        string op = previous().value;
        ParseNode* newNode = new ParseNode{EXPRESSION_NODE, op, {node}};
        newNode->children.push_back(parseComparison());
        node = newNode;
    }
    return node;
}

ParseNode* Parser::parseComparison() {
    ParseNode* node = parseTerm();
    while (match(OPERATOR, "==") || match(OPERATOR, "!=") ||
           match(OPERATOR, "<") || match(OPERATOR, "<=") ||
           match(OPERATOR, ">") || match(OPERATOR, ">=")) {
        string op = previous().value;
        ParseNode* newNode = new ParseNode{EXPRESSION_NODE, op, {node}};
        newNode->children.push_back(parseTerm());
        node = newNode;
    }
    return node;
}

ParseNode* Parser::parseTerm() {
    ParseNode* node = parseFactor();
    while (match(OPERATOR, "+") || match(OPERATOR, "-")) {
        string op = previous().value;
        ParseNode* newNode = new ParseNode{EXPRESSION_NODE, op, {node}};
        newNode->children.push_back(parseFactor());
        node = newNode;
    }
    return node;
}

ParseNode* Parser::parseFactor() {
    ParseNode* node = parsePrimary();
    while (match(OPERATOR, "*") || match(OPERATOR, "/")) {
        string op = previous().value;
        ParseNode* newNode = new ParseNode{EXPRESSION_NODE, op, {node}};
        newNode->children.push_back(parsePrimary());
        node = newNode;
    }
    return node;
}

ParseNode* Parser::parsePrimary() {
    Token t = peek();

    if (match(NUMBER)) {
        return new ParseNode{NUMBER_NODE, previous().value, {}};
    }

    if (match(STRING_LITERAL)) {
        return new ParseNode{EXPRESSION_NODE, previous().value, {}};
    }

    if (match(IDENTIFIER)) {
        return new ParseNode{IDENTIFIER_NODE, previous().value, {}};
    }

    if (match(DELIMITER, "(")) {
        ParseNode* node = parseExpr();
        if (!match(DELIMITER, ")")) error("Expected ')' after expression");
        return node;
    }

    error("Invalid factor");
    return nullptr;
}


// --- Parse Tree Printer ---

void Parser::printParseTree(ParseNode* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; ++i)
        cout << "  ";

    cout << nodeTypeToString(node->type);

    if (!node->value.empty())
        cout << ": " << node->value;

    cout << endl;

    for (ParseNode* child : node->children)
        printParseTree(child, level + 1);
}
