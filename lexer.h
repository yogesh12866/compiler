#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
using namespace std;

enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING_LITERAL,
    OPERATOR, DELIMITER, UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

vector<Token> tokenize(const string& sourceCode);
void printTokens(const vector<Token>& tokens);

#endif
