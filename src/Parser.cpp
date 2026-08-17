#include "Parser.h"
#include "Lox.h"
#include <memory>

Parser::Parser(std::vector<Token> tokens): tokens(tokens) {};

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    try {
        while(!isAtEnd()) {
            statements.push_back(declaration());
        }
        return statements;

    } catch (ParseError error) {
        return {};
    }
}

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if(match({TokenType::Var})) return varDeclaration();

        return statement();
    } catch(ParseError error) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::Identifier, "Expect variable name");
    std::unique_ptr<Expr> initializer ;
    if(match({TokenType::Equal})) initializer = expression();
    consume(TokenType::Semicolon, "Expect ';' after variable declaration.");

    return std::make_unique<Stmt>(Var{name, std::move(initializer)});
}

std::unique_ptr<Stmt> Parser::statement() {
    if(match({TokenType::Print})) return printStatement();
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::Semicolon, "Expect ';' after value.");
    return std::make_unique<Stmt>(Print{std::move(value)});
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::Semicolon, "Expect ';' after value.");
    return std::make_unique<Stmt>(Expression{std::move(value)});
}

Token Parser::advance() {
    if(!isAtEnd()) current++;
    return previous();

}

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality(){
    auto expr = comparison();

    while(match({TokenType::BangEqual, TokenType::EqualEqual})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_unique<Expr>(Binary{std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison(){
    auto expr = term();

    while(match({TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual})) {
        Token op = previous();
        auto right = term();
        expr = std::make_unique<Expr>(Binary{std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term(){
    auto expr = factor();

    while(match({TokenType::Plus, TokenType::Minus})) {
        Token op = previous();
        auto right = factor();
        expr = std::make_unique<Expr>(Binary{std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while(match({TokenType::Slash, TokenType::Star})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<Expr>(Binary{std::move(expr), op, std::move(right)});
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary(){
    if(match({TokenType::Bang, TokenType::Minus})) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<Expr>(Unary{op, std::move(right)});
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary(){
    if (match({TokenType::False})) return std::make_unique<Expr>(Literal{"false"});
    if (match({TokenType::True})) return std::make_unique<Expr>(Literal{"true"});
    if (match({TokenType::Nil})) return std::make_unique<Expr>(Literal{"null"});

    if (match({TokenType::IntLit, TokenType::StrLit})) return std::make_unique<Expr>(Literal{previous().literal});

    if(match({TokenType::Identifier})) return std::make_unique<Expr>(Variable{previous()});

    if (match({TokenType::LeftParen})) {
        auto expr = expression();
        consume(TokenType::RightParen, "Expect ')' after expression");
        return std::make_unique<Expr>(Grouping{std::move(expr)});
    }

    throw error(peek(), "Expected expression");

}


bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType type) {
    if(isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::Eof;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::consume(TokenType type, std::string message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

ParseError Parser::error(Token token, std::string message) {
    Lox::error(token, message);
    return ParseError {};
}
void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::Semicolon) return;

        switch (peek().type) {
            case TokenType::Class:
            case TokenType::Fun:
            case TokenType::Var:
            case TokenType::For:
            case TokenType::If:
            case TokenType::While:
            case TokenType::Print:
            case TokenType::Return:
                return;
            default: advance();
        }
    }
}
