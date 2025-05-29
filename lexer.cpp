#include <vector>
#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>
using namespace std;

#include "lexer.h"

vector<string> keywords = {
    "intt", "sttring", "mainn", "retturn", "iif", "ellse",
    "loop", "brreak", "conttinue", "prrint", "san"
};

bool isKeyword(const string& word) {
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

vector<Token> tokenize(const string& code) {
    vector<Token> tokens;
    int i = 0;
    int len = code.length();

    while (i < len) {
        // Skip whitespace
        if (isspace(code[i])) {
            i++;
            continue;
        }

        // Check for keywords and identifiers
        if (isalpha(code[i]) || code[i] == '_') {
            int start = i;
            while (i < len && (isalnum(code[i]) || code[i] == '_')) i++;
            string word = code.substr(start, i - start);
            if (isKeyword(word)) {
                tokens.push_back({KEYWORD, word});
            } else {
                tokens.push_back({IDENTIFIER, word});
            }
            continue;
        }

        // Check for numbers
        if (isdigit(code[i])) {
            int start = i;
            while (i < len && isdigit(code[i])) i++;
            tokens.push_back({NUMBER, code.substr(start, i - start)});
            continue;
        }

        // Check for string literals
        if (code[i] == '"') {
            int start = i;
            i++; // skip opening quote
            while (i < len && code[i] != '"') i++;
            if (i < len && code[i] == '"') i++; // skip closing quote
            tokens.push_back({STRING_LITERAL, code.substr(start, i - start)});
            continue;
        }

        // Check for multi-character operators
        if (i + 1 < len) {
            string two = code.substr(i, 2);
            if (two == "==" || two == "!=" || two == "<=" || two == ">=" || two == "=>") {
                tokens.push_back({OPERATOR, two});
                i += 2;
                continue;
            }
        }

        // Single-char operators
        char c = code[i];
        if (string("+-*/=<>").find(c) != string::npos) {
            tokens.push_back({OPERATOR, string(1, c)});
            i++;
            continue;
        }

        // Delimiters
        if (string("(){};,").find(c) != string::npos) {
            tokens.push_back({DELIMITER, string(1, c)});
            i++;
            continue;
        }

        // Unknown character
        tokens.push_back({UNKNOWN, string(1, c)});
        i++;
    }

    return tokens;
}

void printTokens(const vector<Token>& tokens) {
    for (const auto& token : tokens) {
        string typeStr;
        switch (token.type) {
            case KEYWORD: typeStr = "KEYWORD"; break;
            case IDENTIFIER: typeStr = "IDENTIFIER"; break;
            case NUMBER: typeStr = "NUMBER"; break;
            case STRING_LITERAL: typeStr = "STRING_LITERAL"; break;
            case OPERATOR: typeStr = "OPERATOR"; break;
            case DELIMITER: typeStr = "DELIMITER"; break;
            default: typeStr = "UNKNOWN"; break;
        }
        cout << "<" << typeStr << ", " << token.value << ">" << endl;
    }
}
