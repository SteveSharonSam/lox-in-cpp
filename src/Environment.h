#include <string>
#include <unordered_map>
#include "Token.h"


class Environment {
    std::unordered_map<std::string, Object> values;

    public:
    void define(std::string name, Object value);
};
