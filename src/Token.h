#pragma once
#include <string>
#include <variant>

using Object = std::variant<std::string, double, std::nullptr_t, bool>;

enum class TokenType {
    //Operators, brackets 
    Plus, Minus, Star, Slash,
    Semicolon, LeftParen, RightParen, LeftBrace, RightBrace, Comma, Dot, 
    Bang, BangEqual, Equal, EqualEqual, Greater, GreaterEqual, Less, LessEqual,

    //Literals
    IntLit, StrLit, Identifier,

    //Keywords
    And, Or, If, Else, Nil, Print, Return, Var, While, For,Super, This, True,Fun,False,Class,
    //Others                                               
    Eof
};

class Token {
    public:
        TokenType type;
        std::string lexeme;
        Object literal;
        int line;
        Token(TokenType t, std::string le, Object li, int line): type(t), lexeme(le), literal(li), line(line) {};
};
