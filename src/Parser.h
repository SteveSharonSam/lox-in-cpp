#pragma once
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include "Scanner.h"
#include "Expr.h"
#include "Stmt.h"

class ParseError: public std::runtime_error {
    public:
        ParseError(): std::runtime_error("") {};
};

class Parser {
    std::vector<Token> tokens;
    int current = 0;

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    bool match(std::initializer_list<TokenType>);
    Token previous();
    bool check(TokenType);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token consume(TokenType, std::string);
    ParseError error(Token, std::string);
    void synchronize();

    std::unique_ptr<Stmt> statement();
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    public: 
        Parser(std::vector<Token> tokens);
        std::vector<std::unique_ptr<Stmt>> parse();
};
