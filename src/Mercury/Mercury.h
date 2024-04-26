//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_MERCURY_H
#define LUNALANG_MERCURY_H

#include <unordered_map>
#include <string>
#include <vector>
#include "Token.h"


namespace Luna::Internal::Mercury {

    static std::unordered_map<std::string, TokenType> keyWords = {
            {"and",      TokenType::And},
            {"_",        TokenType::IgnoredVariable},
            {"class",    TokenType::Class},
            {"else",     TokenType::Else},
            {"False",    TokenType::False},
            {"True",     TokenType::True},
            {"for",      TokenType::For},
            {"break",    TokenType::Break},
            {"continue", TokenType::Continue},
            {"def",      TokenType::Def},
            {"if",       TokenType::If},
            {"None",     TokenType::None},
            {"or",       TokenType::Or},
            {"return",   TokenType::Return},
            {"super",    TokenType::Super},
            {"self",     TokenType::Self},
            {"var",      TokenType::Var},
            {"while",    TokenType::While},
            {"try",      TokenType::Try},
            {"except",   TokenType::Except},
    };

    class Mercury {
    public:
        explicit Mercury(std::string &sourceCode) : sourceCode(sourceCode) {
        }

        std::vector<Token> scanTokens();

    private:
        std::string &sourceCode;
        std::vector<Token> tokens;
        int start = 0;
        int current = 0;
        int line = 1;

        void scanToken();

        [[nodiscard]] bool isAtEnd() const;

        void string();

        void number();

        void identifier();

        static bool isAlpha(char c);

        static bool isDigit(char c);

        static bool isAlphaNumeric(char c);

        void comment();

        char advance();

        void makeToken(TokenType type, void *literal);

        void makeToken(TokenType type, std::string &lexeme);

        void makeToken(TokenType type);

        [[nodiscard]] char peek() const;

        [[nodiscard]] char peekNext() const;

        bool match(char expected);
    };
}

#endif //LUNALANG_MERCURY_H
