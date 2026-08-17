#pragma once
#include "Expr.h"

struct Print;
struct Expression;
struct Var;
struct Stmt;

struct Expression {
    std::unique_ptr<Expr> expression;
};

struct Print {
    std::unique_ptr<Expr> expression;
};

struct Var {
    Token name;
    std::unique_ptr<Expr> initializer;
};

struct Stmt {
    std::variant<Expression, Print, Var> value;
};
