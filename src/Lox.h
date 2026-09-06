#pragma once
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include "Scanner.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Token.h"
#include "RuntimeError.h"
#include "Interpreter.h"


class Lox {
    inline static bool hadError = false;
    inline static bool hadRuntimeError = false;
    static void run(std::string source);
    inline static Interpreter interpreter = Interpreter();
    public : 
    static void report(int line, std::string where, std::string message);
    static void error(Token token, std::string message);
    static void runtimeError(const RuntimeError& error);
    static void runFile(std::string path);
};
