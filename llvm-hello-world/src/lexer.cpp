
#include "lexer.h"

#include <cctype>
#include <utility>

namespace {
int tokenLength(const Token &token) {
    return static_cast<int>(token.lexeme.size());
}
} // namespace

LexerContext::LexerContext(std::string input) : input_(std::move(input)) {}

Token LexerContext::advancePendingToken() {
    if (!pending_tokens_.empty()) {
        Token token = pending_tokens_.front();
        pending_tokens_.pop();
        return token;
    }
    return Token{TokenKind::Unknown, "", {0, 0}};
}

char LexerContext::peek() const {
    if (isAtEnd()) {
        return '\0';
    }
    return input_[index_];
}

char LexerContext::advance() {
    if (isAtEnd()) {
        return '\0';
    }
    char c = input_[index_++];
    if (c == '\n') {
        line_ += 1;
        column_ = 1;
    } else {
        column_ += 1;
    }
    return c;
}

bool LexerContext::isAtEnd() const {
    return index_ >= input_.size();
}

void LexerContext::tryEmitIndentation() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == '\n') {
            advance();
            int new_indentation = 0;
            c = peek();
            while (c == '\t') {
                advance();
                new_indentation += 1;
                c = peek();
            }
            while (current_indentation_ < new_indentation ) {
                pending_tokens_.emplace(Token{});
                current_indentation_ += 1;
            }
        } else {
            break;
        }
    }
}

Token LexerContext::makeSimpleToken(TokenKind kind) {
    SourceLocation loc{line_, column_};
    char c = advance();
    return Token{kind, std::string(1, c), loc};
}

Token LexerContext::lexNumber() {
    SourceLocation loc{line_, column_};
    std::string digits;
    while (std::isdigit(peek())) {
        digits.push_back(advance());
    }
    return Token{TokenKind::Number, digits, loc};
}

Token LexerContext::lexIdentifier() {
    SourceLocation loc{line_, column_};
    std::string text;
    while (std::isalnum(peek()) || peek() == '_') {
        text.push_back(advance());
    }
    return Token{TokenKind::Identifier, text, loc};
}

Token LexerContext::nextToken() {
    tryEmitIndentation();

    Token pendingToken = advancePendingToken();
    if (pendingToken.kind != TokenKind::Unknown) {
        return pendingToken;
    }

    if (isAtEnd()) {
        return Token{TokenKind::End, "", {line_, column_}};
    }

    char c = peek();

    if (std::isdigit(c)) {
        return lexNumber();
    }
    if (std::isalpha(c) || c == '_') {
        return lexIdentifier();
    }

    switch (c) {
        case '+':
            return makeSimpleToken(TokenKind::Plus);
        case '-':
            return makeSimpleToken(TokenKind::Minus);
        case '*':
            return makeSimpleToken(TokenKind::Star);
        case '/':
            return makeSimpleToken(TokenKind::Slash);
        case '(':
            return makeSimpleToken(TokenKind::LParen);
        case ')':
            return makeSimpleToken(TokenKind::RParen);
        case ';':
            return makeSimpleToken(TokenKind::Semicolon);
        default:
            return makeSimpleToken(TokenKind::Unknown);
    }
}

int LexerContext::lex(YYSTYPE *lvalp, YYLTYPE *llocp) {
    Token token = nextToken();

    if (lvalp != nullptr) {
        lvalp->token = token;
    }

    if (llocp != nullptr) {
        llocp->first_line = token.location.line;
        llocp->first_column = token.location.column;
        llocp->last_line = token.location.line;
        int length = tokenLength(token);
        llocp->last_column = length > 0 ? token.location.column + length - 1 : token.location.column;
    }

    return static_cast<int>(token.kind);
}

// yacc-compatible lexer interface
int yylex(YYSTYPE *lvalp, YYLTYPE *llocp, LexerContext &context) {
    return context.lex(lvalp, llocp);
}
