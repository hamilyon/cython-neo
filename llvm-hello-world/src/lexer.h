// Simple hand-rolled lexer scaffolding used by both tests and future parser work.
#ifndef LLVM_HELLO_WORLD_LEXER_H
#define LLVM_HELLO_WORLD_LEXER_H

#include <cstddef>
#include <string>
#include <queue>

enum class TokenKind : int {
    End = 0,
    Def,
    Cdef,
    Identifier,
    Number,
    Plus,
    Minus,
    Star,
    Slash,
    LParen,
    RParen,
    Semicolon,
    LBrace,
    RBrace,
    Unknown,
};

struct SourceLocation {
    int line = 1;
    int column = 1;
};

struct Token {
    TokenKind kind = TokenKind::Unknown;
    std::string lexeme;
    SourceLocation location{};
};

struct YYSTYPE {
    Token token;
};

struct YYLTYPE {
    int first_line = 1;
    int first_column = 1;
    int last_line = 1;
    int last_column = 1;
};

class LexerContext {
public:
    explicit LexerContext(std::string input);

    Token advancePendingToken();

    Token nextToken();
    int lex(YYSTYPE *lvalp, YYLTYPE *llocp);

private:
    char peek() const;
    char advance();
    bool isAtEnd() const;
    void tryEmitIndentation();
    Token lexNumber();
    Token lexIdentifier();
    Token makeSimpleToken(TokenKind kind);

    std::string input_;
    std::size_t index_ = 0;
    int line_ = 1;
    int column_ = 1;

    int current_indentation_ = 0;
    std::queue<Token> pending_tokens_;
};

int yylex(YYSTYPE *lvalp, YYLTYPE *llocp, LexerContext &context);

#endif //LLVM_HELLO_WORLD_LEXER_H
