#include "Lox.h"


void Lox::runFile(std::string path) {
    std::ifstream file(path);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string contents = buffer.str();
    file.close();
    run(contents);
}

void Lox::run(std::string contents) {
    Scanner scanner(contents);
    std::vector<Token> tokens = scanner.scanTokens();
    // for (const Token& token : tokens) {
    //     std::cout << token.line << ":\t" << token.lexeme << "\t"<< static_cast<int>(token.type) << std::endl;
    // }

    //parser takes tokens and creates tree
    Parser parser(tokens);
    auto expr = parser.parse();

    if(hadError) return;

    interpreter.interpret(expr);

}

void Lox::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error " << where
        << ": " << message << " " << std::endl;
}

void Lox::error(Token token, std::string message) {
    if(token.type == TokenType::Eof) {
        report(token.line, "at end", message);
    } else {
        report(token.line, "at",message);
    }
}

void Lox::runtimeError(const RuntimeError &error) {
    std::cerr << error.what() << "\n[line " << error.token.line << "]" << std::endl ;
    hadRuntimeError = true;
}
