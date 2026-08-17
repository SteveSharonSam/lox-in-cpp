#pragma once
#include <vector>
#include "Expr.h"
#include "Stmt.h"


class Interpreter {
        Object evaluate(const Expr& expr) {
            return std::visit(*this, expr.value);
        }
        bool isTruthy(const Object& obj);
        bool isEqual(const Object& left, const Object& right);
        void checkNumberOperand(const Token& op, const Object& operand);
        void checkNumberOperand(const Token& op, const Object& right, const Object& left);
        std::string stringify(const Object& obj);
        void execute(const Stmt& stmt) {
            std::visit(*this, stmt.value);
        }

    public:
        Object operator()(const Literal& expr);
        Object operator()(const Grouping& expr);
        Object operator()(const Unary& expr);
        Object operator()(const Binary& expr);
        Object operator()(const Variable& expr);
        void operator() (const Print& stmt);
        void operator() (const Expression& stmt);
        void operator() (const Var& stmt);
        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
};
