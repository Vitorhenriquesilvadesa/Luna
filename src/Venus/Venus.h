//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_VENUS_H
#define LUNALANG_VENUS_H

#pragma once

#include <vector>
#include <memory>

#include "../Mercury/Token.h"
#include "Statement.h"
#include "Expression.h"

using namespace Luna::Internal::Mercury;

namespace Luna::Internal::Venus {

    enum class FunctionType { Function, Method };

    class Venus {
    public:
        explicit Venus(const std::vector<Token> &tokens) : tokens(tokens) {

        }

        std::vector<std::shared_ptr<Statement>> parseTokens();

    private:
        std::shared_ptr<Statement> expressionStatement();

        std::shared_ptr<Expression> expression();

        std::shared_ptr<Expression> assignmentExpression();

        std::shared_ptr<Expression> ternaryExpression();

        std::shared_ptr<Expression> orExpression();

        std::shared_ptr<Expression> andExpression();

        std::shared_ptr<Expression> equalityExpression();

        std::shared_ptr<Expression> comparisionExpression();

        std::shared_ptr<Expression> termExpression();

        std::shared_ptr<Expression> factorExpression();

        std::shared_ptr<Expression> unaryExpression();

        std::shared_ptr<Expression> callExpression();

        std::shared_ptr<Expression> primaryExpression();

        bool isAtEnd();

        bool match(std::initializer_list<TokenType> types);

        bool check(TokenType type);

        Token advance();

        Token peek();

        Token previous();

        Token consume(TokenType type, const char* message);
        Token consume(TokenType type, const std::string& message);

        void synchronize();


        std::vector<Token> tokens;
        int current = 0;

        std::shared_ptr<Expression> finishCall(std::shared_ptr<Expression> callee);

        std::shared_ptr<Statement> declarationStatement();

        std::shared_ptr<Statement> classDeclaration();

        std::shared_ptr<Statement> functionDeclaration(const char *string, FunctionType type, bool isStatic);

        std::shared_ptr<Statement> varDeclaration();

        std::shared_ptr<Statement> statement();

        std::vector<std::shared_ptr<Statement>> block();

        std::shared_ptr<Statement> forStatement();

        std::shared_ptr<Statement> ifStatement();

        std::shared_ptr<Statement> whileStatement();

        std::shared_ptr<Statement> returnStatement();

        std::shared_ptr<Statement> initializerDeclaration();

        void error(const char *message);
    };
}

#endif //LUNALANG_VENUS_H
