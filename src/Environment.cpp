#include "Environment.h"
#include "RuntimeError.h"

void Environment::define(std::string name, Object value) {
    values[name] = value;
}

void Environment::assign(Token name, Object value) {
    if(values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    if(enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable'" + name.lexeme + "'.");
}

Object Environment::get(Token name) {
    if(values.contains(name.lexeme)) {
        return values[name.lexeme];
    }

    if(enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable'" + name.lexeme + "'.");
}
