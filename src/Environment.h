#include <string>
#include <unordered_map>
#include "Token.h"


class Environment {
    std::unordered_map<std::string, Object> values;
    Environment* enclosing;

    public:
    Environment(): enclosing(nullptr) {}
    Environment(Environment& enclosing): enclosing(&enclosing) {}
    void define(std::string name, Object value);
    void assign(Token name, Object value);
    Object get(Token name);
};
