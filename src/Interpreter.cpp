#include "Interpreter.h"
#include "Expr.h"
#include "Token.h"
#include "RuntimeError.h"
#include <iostream>
#include <string>
#include <variant>
#include <vector>
Object Interpreter::operator()(const Literal& expr) {
    return expr.value;
}

Object Interpreter::operator()(const Grouping& expr) {
    return evaluate(*expr.expression);
}

Object Interpreter::operator()(const Unary& expr) {
    Object right = evaluate(*expr.right);
    switch (expr.op.type) {
        case TokenType::Bang :
            return !isTruthy(right);
        case TokenType::Minus : 
            checkNumberOperand(expr.op, right);
            return -std::get<double>(right) ;
        default : 
            return std::get<double>(right);
    }

    return {};
}

Object Interpreter::operator()(const Binary& expr) {
    Object left = evaluate(*expr.left);
    Object right = evaluate(*expr.right);

    switch (expr.op.type) {
        case TokenType::Greater :
            checkNumberOperand(expr.op, right, left);
            return (std::get<double>(left) > std::get<double>(right));
        case TokenType::GreaterEqual :
            checkNumberOperand(expr.op, right, left);
            return (std::get<double>(left) >= std::get<double>(right));
        case TokenType::Less :
            checkNumberOperand(expr.op, right, left);
            return (std::get<double>(left) < std::get<double>(right));
        case TokenType::LessEqual :
            checkNumberOperand(expr.op, right, left);
            return (std::get<double>(left) <= std::get<double>(right));
        case TokenType::Minus :
            checkNumberOperand(expr.op, right, left);
            return (std::get<double>(left) - std::get<double>(right));
        case TokenType::Plus :
            if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                return (std::get<double>(left) + std::get<double>(right));
            if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
                return (std::get<std::string>(left) + std::get<std::string>(right));
            if(std::holds_alternative<std::string>(left) && std::holds_alternative<double>(right))
                return (std::get<std::string>(left) + stringify(std::get<double>(right)));
            if(std::holds_alternative<double>(left) && std::holds_alternative<std::string>(right))
                return (stringify(std::get<double>(left)) + std::get<std::string>(right));
            throw RuntimeError(expr.op, "Operands must be either number or string.");
        case TokenType::Slash :
            checkNumberOperand(expr.op, right, left);
            if(std::get<double>(right) == 0) throw RuntimeError(expr.op, "Division by zero is not allowed.");
            return (std::get<double>(left) / std::get<double>(right));
        case TokenType::Star :
            checkNumberOperand(expr.op, right, left);
            return (std::get<double>(left) * std::get<double>(right));
        case TokenType::BangEqual : 
            return !isEqual(left, right);
        case TokenType::EqualEqual :
            return isEqual(left, right);
        default:
            break;
    }

    return {};
}

Object Interpreter::operator()(const Variable& expr) {
    return environment->get(expr.name);
}

Object Interpreter::operator()(const Assign& expr) {
    Object value = evaluate(*expr.value);
    environment->assign(expr.name, value);
    return value;
}

void Interpreter::operator()(const Print& stmt) {
    Object value = evaluate(*stmt.expression);
    std::cout << stringify(value) << std::endl;
    return;
}

void Interpreter::operator()(const Expression& stmt) {
    evaluate(*stmt.expression);
    return;
}

void Interpreter::operator()(const Var& stmt) {
    Object value = nullptr;
    if(stmt.initializer != NULL) {
        value = evaluate(*stmt.initializer);
    }
    environment->define(stmt.name.lexeme, value);
    return;
}

void Interpreter::operator()(const Block& stmt) {
    Environment currBlockEnv = Environment(*environment);
    executeBlock(stmt.statements, currBlockEnv);
    return;
}

void Interpreter::executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, Environment& env) {
    Environment *prev = this->environment;

    try {
        this->environment = &env;

        for(auto& statement: statements) {
            execute(*statement);
        }
    } catch(const RuntimeError& e) {
        this->environment = prev;
        throw;
    }
    this->environment = prev;

}

bool Interpreter::isTruthy(const Object& object) {
    if (std::holds_alternative<std::nullptr_t>(object))
        return false;

    if (std::holds_alternative<bool>(object))
        return std::get<bool>(object);

    return true;
}

bool Interpreter::isEqual(const Object& left, const Object& right) {
    return right == left;

}
void Interpreter::checkNumberOperand(const Token& op, const Object& operand) {
    if(std::holds_alternative<double>(operand)) return;
    throw RuntimeError(op, "Operand must be number.");

}


void Interpreter::checkNumberOperand(const Token& op, const Object& right, const Object& left) {
    if(std::holds_alternative<double>(right) && std::holds_alternative<double>(left) ) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements) {
    try {
        for(auto& stmt : statements) {
            execute(*stmt);
        }
    } catch(RuntimeError e) {
        std::cout << e.what() << std::endl;
    }
}

std::string Interpreter::stringify(const Object& object) {
    if(std::holds_alternative<std::nullptr_t>(object)) return "nil";

    if(std::holds_alternative<double>(object)) {
        std::string text = std::to_string(std::get<double>(object));
        
        if(text.ends_with(".000000")) {
            text.erase(text.size()-7);
        }

        return text;
    }

    if(std::holds_alternative<bool>(object)) {
        return std::get<bool>(object) ? "true" : "false";
    }

    return std::get<std::string>(object);
}
