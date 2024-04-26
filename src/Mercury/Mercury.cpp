//
// Created by vitor on 24/04/24.
//

#include <iostream>
#include "Mercury.h"
#include "../Base/LunaBaseType.h"
#include "../Error/Error.h"


std::vector<Luna::Internal::Mercury::Token> Luna::Internal::Mercury::Mercury::scanTokens() {

    tokens.clear();

    std::cout << "Scanning file." << std::endl;
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::EndOfFile, "", line);
    return tokens;
}

void Luna::Internal::Mercury::Mercury::scanToken() {
    switch (const char c = advance()) {
        case '(':
            makeToken(TokenType::LeftParen);
            break;
        case ')':
            makeToken(TokenType::RightParen);
            break;
        case '{':
            makeToken(TokenType::LeftBrace);
            break;
        case '}':
            makeToken(TokenType::RightBrace);
            break;
        case '[':
            makeToken(TokenType::LeftBracket);
            break;
        case ']':
            makeToken(TokenType::RightBracket);
            break;
        case ',':
            makeToken(TokenType::Comma);
            break;
        case '.':
            makeToken(TokenType::Dot);
            break;
        case '-':
            makeToken(TokenType::Minus);
            break;
        case '+':
            makeToken(TokenType::Plus);
            break;
        case ';':
            makeToken(TokenType::Semicolon);
            break;
        case '*':
            makeToken(TokenType::Star);
            break;
        case '!':
            makeToken(match('=') ? TokenType::BangEqual : TokenType::Bang);
            break;
        case '=':
            makeToken(match('=') ? TokenType::EqualEqual : TokenType::Equal);
            break;
        case '<':
            makeToken(match('=') ? TokenType::LessEqual : TokenType::Less);
            break;
        case '>':
            makeToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
            break;

        case ':':
            makeToken(match(':') ? TokenType::DoubleColon : TokenType::Colon);
            break;

        case '?':
            makeToken(match('?') ? TokenType::DoubleQuestion : TokenType::Question);
            break;

        case '/':
            makeToken(TokenType::Slash);
            break;

        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            makeToken(TokenType::NewLine);
            break;

        case '"':
            string();
            break;

        case '@':
            makeToken(TokenType::Decorator);
            break;

        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                ERROR("Unexpected character at line " + std::to_string(line));
            }
            break;
    }
}

bool Luna::Internal::Mercury::Mercury::isAtEnd() const {
    return current >= sourceCode.length() || sourceCode[current] == '\0';
}

void Luna::Internal::Mercury::Mercury::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        ERROR("Unterminated string.");
    }

    advance();

    std::string value = sourceCode.substr(start + 1, current - start - 2);
    makeToken(TokenType::String, value);
}

void Luna::Internal::Mercury::Mercury::number() {
    bool isFloat = false;

    while (!isAtEnd() && (isDigit(peek()) || peek() == '.')) {
        if (peek() == '.') {
            if (isFloat) {
                ERROR("Error to parse number at line " + std::to_string(line));
            }
            isFloat = true;
        }

        advance();

        if (isFloat && peek() == '.') {
            ERROR("Error to parse number at line " + std::to_string(line));
        }
    }

    std::string number = sourceCode.substr(start, current - start);
    double parsedNumber = std::stod(number);

    makeToken(TokenType::Double, number);
}

void Luna::Internal::Mercury::Mercury::identifier() {
    while (isAlphaNumeric(peek())) advance();
    const std::string text = sourceCode.substr(start, current - start);
    TokenType type;

    try {
        type = keyWords.at(text);
    } catch ([[maybe_unused]] std::out_of_range &e) {
        type = TokenType::Identifier;
    }

    makeToken(type);
}

bool Luna::Internal::Mercury::Mercury::isAlpha(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Luna::Internal::Mercury::Mercury::isDigit(const char c) {
    return c >= '0' && c <= '9';
}

bool Luna::Internal::Mercury::Mercury::isAlphaNumeric(const char c) {
    return isAlpha(c) || isDigit(c);
}

void Luna::Internal::Mercury::Mercury::comment() {
    while (peek() != '\n' && !isAtEnd()) advance();
}

char Luna::Internal::Mercury::Mercury::advance() {
    if (isAtEnd()) return '\0';
    return sourceCode[current++];
}


void Luna::Internal::Mercury::Mercury::makeToken(TokenType type) {
    std::string text = sourceCode.substr(start, current - start);
    this->tokens.emplace_back(type, text, line);
}

void Luna::Internal::Mercury::Mercury::makeToken(TokenType type, std::string &lexeme) {
    this->tokens.emplace_back(type, lexeme, line);
}

char Luna::Internal::Mercury::Mercury::peek() const {
    if (isAtEnd()) return '\0';
    return sourceCode[current];
}

char Luna::Internal::Mercury::Mercury::peekNext() const {
    if (current + 1 >= sourceCode.length()) return '\0';
    return sourceCode[current + 1];
}

bool Luna::Internal::Mercury::Mercury::match(const char expected) {
    if (isAtEnd()) return false;
    if (sourceCode[current] != expected) return false;
    current++;
    return true;
}
