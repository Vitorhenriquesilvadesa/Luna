//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_TOKEN_H
#define LUNALANG_TOKEN_H

#include <string>
#include <unordered_map>
#include "../Base/LunaBaseType.h"


namespace Luna::Internal::Mercury {

    enum class TokenType {
        LeftParen, RightParen, LeftBrace, RightBrace, LeftBracket, RightBracket, Question, DoubleQuestion,
        Comma, Dot, Minus, Plus, Colon, DoubleColon, Semicolon, Slash, Star,

        // One or two character tokens.
        Bang, BangEqual,
        Equal, EqualEqual,
        Greater, GreaterEqual,
        Less, LessEqual,

        // Literals.
        Identifier, String, Int, Float,

        // Keywords.
        And, Class, Else, False, Def, For, If, None, Or, Break, Continue,
        Return, Super, Self, True, Var, While, Namespace, Abstract,
        EndOfFile, Decorator, Try, Except, With, Open, As, Import, Double, NewLine, IgnoredVariable,
    };

    static const std::unordered_map<TokenType, std::string> tokenStringMap = {
            {TokenType::LeftParen,       "LeftParen"},
            {TokenType::IgnoredVariable, "_"},
            {TokenType::NewLine,         "NewLine"},
            {TokenType::RightParen,      "RightParen"},
            {TokenType::LeftBrace,       "LeftBrace"},
            {TokenType::RightBrace,      "RightBrace"},
            {TokenType::LeftBracket,     "LeftBracket"},
            {TokenType::RightBracket,    "RightBracket"},
            {TokenType::Question,        "Question"},
            {TokenType::DoubleQuestion,  "DoubleQuestion"},
            {TokenType::Comma,           "Comma"},
            {TokenType::Dot,             "Dot"},
            {TokenType::Minus,           "Minus"},
            {TokenType::Plus,            "Plus"},
            {TokenType::Colon,           "Colon"},
            {TokenType::DoubleColon,     "DoubleColon"},
            {TokenType::Semicolon,       "Semicolon"},
            {TokenType::Slash,           "Slash"},
            {TokenType::Star,            "Star"},
            {TokenType::Bang,            "Bang"},
            {TokenType::BangEqual,       "BangEqual"},
            {TokenType::Equal,           "Equal"},
            {TokenType::EqualEqual,      "EqualEqual"},
            {TokenType::Greater,         "Greater"},
            {TokenType::GreaterEqual,    "GreaterEqual"},
            {TokenType::Less,            "Less"},
            {TokenType::LessEqual,       "LessEqual"},
            {TokenType::Identifier,      "Identifier"},
            {TokenType::String,          "String"},
            {TokenType::Double,          "Double"},
            {TokenType::And,             "And"},
            {TokenType::Class,           "Class"},
            {TokenType::Else,            "Else"},
            {TokenType::False,     "False"},
            {TokenType::Def,       "Def"},
            {TokenType::For,       "For"},
            {TokenType::If,        "If"},
            {TokenType::None,      "None"},
            {TokenType::Or,        "Or"},
            {TokenType::Break,     "Break"},
            {TokenType::Continue,  "Continue"},
            {TokenType::Return,    "Return"},
            {TokenType::Super,     "Super"},
            {TokenType::Self,      "Self"},
            {TokenType::True,      "True"},
            {TokenType::Var,       "Var"},
            {TokenType::While,     "While"},
            {TokenType::Namespace, "Namespace"},
            {TokenType::Abstract,  "Abstract"},
            {TokenType::Decorator, "Decorator"},
            {TokenType::EndOfFile, "EndOfFile"},
            {TokenType::Try,       "Try"},
            {TokenType::Except,    "Except"},
            {TokenType::With,      "With"},
            {TokenType::Open,            "Open"},
            {TokenType::As,              "As"},
            {TokenType::Import,          "Import"},
    };

    static std::string TokenTypeToString(const TokenType type) {
        return tokenStringMap.at(type);
    }

    struct Token final {
        Token(const TokenType type, std::string lexeme, const unsigned int line)
                : type(type),
                  lexeme(std::move(lexeme)),
                  line(line) {
        }

        const TokenType type;
        const std::string lexeme;
        const unsigned int line;

        [[nodiscard]] std::string toString() const {
            return "TokenType { " + TokenTypeToString(type) + ", '" + lexeme + "'" + " }";
        }

        ~Token() = default;
    };

} // Internal
// Luna

#endif //LUNALANG_TOKEN_H
