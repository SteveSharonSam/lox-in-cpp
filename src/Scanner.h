#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Token.h"
class Scanner {
    std::string source;
    std::vector<Token> tokens;

    bool AtEnd() const;
    char advance();
    void scanToken();
    void addToken(TokenType);
    void addToken(TokenType, Object);
    bool match(char);
    char peek();
    char peekNext();
    void handleString();
    void number();
    void identifier();
    void multiLineComment();

    std::unordered_map<std::string, TokenType> keywords;

    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int currentLine = 1;
    unsigned int line = 1;
    


    public:
    Scanner(const std::string& source);
    std::vector<Token> scanTokens();
    ~Scanner();
};
