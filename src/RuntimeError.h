#include "Token.h"
#include <stdexcept>


class RuntimeError : public std::runtime_error {
    public:
        Token token;
        RuntimeError(const Token& token, const std::string& message): token(token), std::runtime_error(message) {}
};
