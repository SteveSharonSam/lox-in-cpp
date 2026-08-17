#pragma once
#include "Token.h"
#include <cstddef>
#include <memory>

struct Expr;
struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Variable;


struct Literal {
    Object value;
};

struct Grouping {
    std::unique_ptr<Expr> expression;
};

struct Unary {
    Token op;
    std::unique_ptr<Expr> right;
};

struct Binary {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct Variable {
    Token name;
};

struct Expr {
    std::variant<Binary, Unary, Grouping, Literal, Variable> value; 
}; 
