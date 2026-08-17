#include "Utils.h"

void Utils::isError(int line, std::string message) {
    std::cout << line << " : " << message << std::endl;
}

bool Utils::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Utils::isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

bool Utils::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

