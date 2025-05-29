
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "lexer.h"

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case KEYWORD: return "KEYWORD";
        case IDENTIFIER: return "IDENTIFIER";
        case NUMBER: return "NUMBER";
        case STRING_LITERAL: return "STRING_LITERAL";
        case OPERATOR: return "OPERATOR";
        case DELIMITER: return "DELIMITER";
        case UNKNOWN: return "UNKNOWN";
        default: return "UNDEFINED";
    }
}

#endif 
