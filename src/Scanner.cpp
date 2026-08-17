#include "Scanner.h"
#include "Utils.h"
#include <string>

Scanner::Scanner(const std::string& source): source(source) {
  keywords.insert({
    {"and",    TokenType::And},
    {"class",  TokenType::Class},
    {"else",   TokenType::Else},
    {"false",  TokenType::False},
    {"for",    TokenType::For},
    {"fun",    TokenType::Fun},
    {"if",     TokenType::If},
    {"nil",    TokenType::Nil},
    {"or",     TokenType::Or},
    {"print",  TokenType::Print},
    {"return", TokenType::Return},
    {"super",  TokenType::Super},
    {"this",   TokenType::This},
    {"true",   TokenType::True},
    {"var",    TokenType::Var},
    {"while",  TokenType::While}
  });
}

Scanner::~Scanner() {}

bool Scanner::AtEnd() const {
    return current >= source.size();
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(' : addToken(TokenType::LeftParen); break;
        case ')' : addToken(TokenType::RightParen); break;
        case '{' : addToken(TokenType::LeftBrace); break;
        case '}' : addToken(TokenType::RightBrace); break;
        case '+' : addToken(TokenType::Plus); break;
        case '-' : addToken(TokenType::Minus); break;
        case '*' : addToken(TokenType::Star); break;
        case '/' : 
                   if(match('/')) {
                       while( peek() != '\n' && !AtEnd()) advance();
                   } else if (match('*')) {
                        multiLineComment();
                   } else 
                       addToken(TokenType::Slash);
                   break;
        case ';' : addToken(TokenType::Semicolon); break;
        case ',' : addToken(TokenType::Comma); break;
        case '.' : addToken(TokenType::Dot); break;
        case '!' : addToken(match('=') ? TokenType::BangEqual: TokenType::Bang); break;
        case '=' : addToken(match('=') ? TokenType::EqualEqual: TokenType::Equal); break;
        case '<' : addToken(match('=') ? TokenType::LessEqual : TokenType::Less); break;
        case '>' : addToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater); break;   
        case '"' : handleString(); break;
        case ' ' : 
        case '\r':
        case '\t': break;
        case '\n': line++; break;
        default: 
                   if (Utils::isDigit(c)) {
                       number();
                       break;
                   } else if(Utils::isAlpha(c)) {
                       identifier();
                       break;
                   } else {
                       Utils::isError(line, "Unexpected characted {number}");
                       break;
                   }
                   Utils::isError(line, "{default}Unexpected character");
                   break;
    }

}

char Scanner::advance() {
    return source[current++];
}

bool Scanner::match(char c) {
    if (AtEnd()) return false;
    if (source[current] != c) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (AtEnd()) return '\0';
    return source[current];
}

char Scanner::peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

void Scanner::handleString() {
    while( peek() != '"' && !AtEnd()) {
        if (peek() == '\n') 
            line++;
        advance();
    }

    if (AtEnd()) {
        Utils::isError(line, "Unterminated string");
        return ;
    }

    advance();

    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::StrLit, value);
}

void Scanner::number() {
   while (Utils::isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && Utils::isDigit(peekNext())) {
      // Consume the "."
      advance();

      while (Utils::isDigit(peek())) advance();
    }

    //need to add tokentype with value in float
    float num = std::stof(source.substr(start, current - start + 1));
    addToken(TokenType::IntLit, num);
}

void Scanner::identifier() {
    while(Utils::isAlphaNumeric(peek())) advance();

    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::Identifier;

    addToken(type);
}

void Scanner::multiLineComment() {
    //continue till */ if there is newline in the comment increment line
    while(peek()!='*' && peekNext()!='/' && !AtEnd()) {
        if (advance() == '\n') {
            line++;
        }
    }
    advance();
    advance();
}

void Scanner::addToken(TokenType type) {
    Object null = "";
    addToken(type, null);
}
void Scanner::addToken(TokenType type, Object literal) {
    std::string str = source.substr(start, current - start);
    tokens.push_back(Token(type, str, literal, line));
} 

std::vector<Token> Scanner::scanTokens() {
    while(!AtEnd()) {
        start = current;
        //scan for tokens and update positions accordingly
        scanToken();

    }

    //add eof to token vector
    tokens.push_back(Token(TokenType::Eof, "", "", line));
    return tokens;
}
