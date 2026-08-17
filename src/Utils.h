#pragma once 
#include <string>
#include <iostream>
#include "Token.h"

namespace Utils {
    void isError(int line, std::string message);
    bool isDigit(char);
    bool isAlpha(char);
    bool isAlphaNumeric(char);
    // void report(int, std::string, std::string);
    // void error(Token, std::string);
}
