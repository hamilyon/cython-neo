#include "lexer.h"

#include <iostream>
#include <string>
#include <vector>

struct TestFailure {
    std::string message;
};

class TestRunner {
public:
    void expect(bool condition, const std::string &message) {
        if (!condition) {
            failures_.push_back({message});
        }
    }

    bool hasFailures() const {
        return !failures_.empty();
    }

    int report() const {
        if (failures_.empty()) {
            std::cout << "All lexer tests passed.\n";
            return 0;
        }
        std::cerr << "Lexer tests failed (" << failures_.size() << "):\n";
        for (const auto &failure : failures_) {
            std::cerr << "  - " << failure.message << "\n";
        }
        return 1;
    }

private:
    std::vector<TestFailure> failures_;
};

static std::vector<Token> collectTokens(const std::string &input) {
    LexerContext ctx(input);
    std::vector<Token> tokens;
    for (;;) {
        Token t = ctx.nextToken();
        tokens.push_back(t);
        if (t.kind == TokenKind::End) {
            break;
        }
    }
    return tokens;
}

static void testSimpleExpression(TestRunner &runner) {
    auto tokens = collectTokens("foo + 123;");
    runner.expect(tokens.size() == 5, "Expected 5 tokens including EOF");

    runner.expect(tokens[0].kind == TokenKind::Identifier && tokens[0].lexeme == "foo",
                  "Identifier token should be 'foo'");
    runner.expect(tokens[1].kind == TokenKind::Plus, "Second token should be Plus");
    runner.expect(tokens[2].kind == TokenKind::Number && tokens[2].lexeme == "123",
                  "Third token should be number 123");
    runner.expect(tokens[3].kind == TokenKind::Semicolon, "Fourth token should be Semicolon");
    runner.expect(tokens[4].kind == TokenKind::End, "Last token should be End");

    runner.expect(tokens[0].location.line == 1 && tokens[0].location.column == 1,
                  "Identifier should start at column 1");
    runner.expect(tokens[1].location.column == 5, "Plus should start after single space");
    runner.expect(tokens[2].location.column == 7, "Number should start after plus and space");
}

static void testNewlinesAndUnknown(TestRunner &runner) {
    auto tokens = collectTokens("abc\n$\n45");
    runner.expect(tokens[0].location.line == 1 && tokens[0].location.column == 1,
                  "First identifier location should be line 1, column 1");
    runner.expect(tokens[1].kind == TokenKind::Unknown && tokens[1].lexeme == "$",
                  "Unknown character should be returned as Unknown token");
    runner.expect(tokens[1].location.line == 2 && tokens[1].location.column == 1,
                  "Unknown token location should reflect newline");
    runner.expect(tokens[2].kind == TokenKind::Number && tokens[2].lexeme == "45",
                  "Number token should parse after newline");
    runner.expect(tokens.back().kind == TokenKind::End, "EOF token should be present");
}

int main() {
    TestRunner runner;
    testSimpleExpression(runner);
    testNewlinesAndUnknown(runner);
    return runner.report();
}
