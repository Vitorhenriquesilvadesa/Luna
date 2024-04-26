//
// Created by vitor on 24/04/24.
//

#include "Venus.h"
#include <iostream>

template<typename T>
using Ref = std::shared_ptr<T>;


namespace Luna::Internal::Venus {
    std::vector<Ref < Statement>>

    Luna::Internal::Venus::Venus::parseTokens() {
        std::vector<Ref<Statement>> statements = std::vector<Ref<Statement>>();

        while (!isAtEnd()) {
            if (match({TokenType::NewLine})) {
                continue;
            }
            statements.push_back(declarationStatement());
        }

        return statements;
    }


    Ref <Statement> Venus::expressionStatement() {
        Ref<Expression> expr = expression();
        consume(TokenType::NewLine, "Expect new line after expression.");
        return std::make_shared<ExpressionStatement>(expr);
    }

    Ref <Expression> Venus::expression() {
        if (match({TokenType::Semicolon})) {
            error("Expression cannot be start with ';'.");
        }
        return assignmentExpression();
    }

    Ref <Expression> Venus::assignmentExpression() { // NOLINT(*-no-recursion)
        Ref<Expression> expr = ternaryExpression();

        if (match({TokenType::Equal})) {
            Token equals = previous();
            Ref<Expression> value = assignmentExpression();

            if (auto variableExpr = std::dynamic_pointer_cast<VariableExpression>(expr)) {
                Token name = variableExpr->name;
                return std::make_shared<AssignmentExpression>(name, value);
            } else if (auto getExpr = std::dynamic_pointer_cast<GetExpression>(expr)) {
                return std::make_shared<SetExpression>(getExpr->name, getExpr->object, value);
            }

            error("Invalid assignment target.");
        }

        return expr;
    }

    Ref <Expression> Venus::ternaryExpression() {
        Ref<Expression> condition = orExpression();

        if (match({TokenType::Question})) {
            Ref<Expression> left = expression();
            consume(TokenType::Colon, "Expect ':' after expression in ternary operator.");
            Ref<Expression> right = expression();
            return std::make_shared<TernaryExpression>(condition, left, right);
        }

        return condition;
    }

    Ref <Expression> Venus::orExpression() {
        Ref<Expression> expression = andExpression();

        while (match({TokenType::Or})) {
            Token exprOperator = previous();
            Ref<Expression> right = andExpression();
            expression = std::make_shared<LogicalExpression>(expression, right, exprOperator);
        }

        return expression;
    }

    Ref <Expression> Venus::andExpression() {
        Ref<Expression> expression = equalityExpression();

        while (match({TokenType::And})) {
            Token exprOperator = previous();
            Ref<Expression> right = equalityExpression();
            expression = std::make_shared<LogicalExpression>(expression, right, exprOperator);
        }

        return expression;
    }

    Ref <Expression> Venus::equalityExpression() {
        Ref<Expression> expression = comparisionExpression();

        while (match({TokenType::BangEqual, TokenType::EqualEqual})) {
            Token exprOperator = previous();
            Ref<Expression> right = comparisionExpression();
            expression = std::make_shared<BinaryExpression>(expression, right, exprOperator);
        }

        return expression;
    }

    Ref <Expression> Venus::comparisionExpression() {
        Ref<Expression> expression = termExpression();

        while (match({TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual})) {
            Token exprOperator = previous();
            Ref<Expression> right = termExpression();
            expression = std::make_shared<BinaryExpression>(expression, right, exprOperator);
        }

        return expression;
    }

    Ref <Expression> Venus::termExpression() {
        Ref<Expression> expression = factorExpression();

        while (match({TokenType::Minus, TokenType::Plus})) {
            Token exprOperator = previous();
            Ref<Expression> right = factorExpression();
            expression = std::make_shared<BinaryExpression>(expression, right, exprOperator);
        }

        return expression;
    }

    Ref <Expression> Venus::factorExpression() {
        Ref<Expression> expr = unaryExpression();

        while (match({TokenType::Slash, TokenType::Star})) {
            Token exprOperator = previous();
            Ref<Expression> right = factorExpression();
            expr = std::make_shared<BinaryExpression>(expr, right, exprOperator);
        }

        return expr;
    }

    Ref <Expression> Venus::unaryExpression() {
        if (match({TokenType::Bang, TokenType::Minus})) {
            Token exprOperator = previous();
            Ref<Expression> right = unaryExpression();
            return std::make_shared<UnaryExpression>(exprOperator, right);
        }

        return callExpression();
    }

    Ref <Expression> Venus::callExpression() {
        Ref<Expression> expression = primaryExpression();

        while (true) {
            if (match({TokenType::LeftParen})) {
                expression = finishCall(expression);
            } else if (match({TokenType::Dot})) {
                Token name = consume(TokenType::Identifier, "Expect property name after '.'.");
                expression = std::make_shared<GetExpression>(expression, name);
            } else {
                break;
            }
        }

        return expression;
    }

    Ref <Expression> Venus::finishCall(Ref <Expression> callee) {
        std::vector<Ref<Expression>> arguments = std::vector<Ref<Expression>>();

        if (!check(TokenType::RightParen)) {
            do {
                if (arguments.size() >= 255) {
                    error("Can't have more than 255 arguments.");
                }
                arguments.push_back(expression());
            } while (match({TokenType::Comma}));
        }

        Token paren = consume(TokenType::RightParen, "Expect ')' after arguments.");

        return std::make_shared<CallExpression>(callee, paren, arguments);
    }

    Ref <Expression> Venus::primaryExpression() {
        if (match({TokenType::False})) {
            return std::make_shared<LiteralExpression<bool>>(false);
        }
        if (match({TokenType::True, TokenType::IgnoredVariable})) {
            return std::make_shared<LiteralExpression<bool>>(true);
        }
        if (match({TokenType::None})) {
            // TODO make void type.
            return std::make_shared<LiteralExpression<int>>(0);
        }

        if (match({TokenType::Double})) {
            double number = std::stod(previous().lexeme);
            return std::make_shared<LiteralExpression<double>>(number);
        }
        if (match({TokenType::String})) {
            return std::make_shared<LiteralExpression<const char *>>(previous().lexeme.c_str());
        }

        if (match({TokenType::Super})) {
            Token keyword = previous();
            consume(TokenType::Dot, "Expect '.' after super.");
            Token method = consume(TokenType::Identifier, "Expect superclass method name.");
            return std::make_shared<SuperExpression>(keyword, method);
        }

        if (match({TokenType::Self})) {
            return std::make_shared<SelfExpression>(previous());
        }

        if (match({TokenType::Identifier})) {
            return std::make_shared<VariableExpression>(previous());
        }

        if (match({TokenType::LeftParen})) {
            Ref<Expression> expr = expression();
            consume(TokenType::RightParen, "Expect ')' after expression.");
            return std::make_shared<GroupingExpression>(expr);
        }

        if (match({TokenType::Break})) {
            return std::make_shared<BreakExpression>();
        }

        error("Expect expression");
        return nullptr;
    }

    bool Venus::isAtEnd() {
        return peek().type == TokenType::EndOfFile;
    }

    bool Venus::match(std::initializer_list<TokenType> types) {
        for (const TokenType &type: types) {
            if (check(type)) {
                advance();
                return true;
            }
        }

        return false;
    }

    bool Venus::check(TokenType type) {
        if (isAtEnd()) {
            return false;
        }

        return peek().type == type;
    }

    Token Venus::advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    Token Venus::peek() {
        return tokens[current];
    }

    Token Venus::previous() {
        return tokens[current - 1];
    }

    Token Venus::consume(TokenType type, const char *message) {
        if (check(type)) return advance();
        std::cerr << message << " At line " << std::to_string(peek().line) << std::endl;
        throw std::exception();
    }

    void Venus::error(const char *message) {
        std::cerr << message << " At line " << std::to_string(peek().line) << std::endl;
        throw std::exception();
    }

    Token Venus::consume(TokenType type, const std::string &message) {
        if (check(type)) return advance();
        std::cerr << message << " At line " << std::to_string(peek().line) << std::endl;
        throw std::exception();
    }

    void Venus::synchronize() {

    }

    Ref <Statement> Venus::declarationStatement() {
        try {
            if (match({TokenType::Class})) {
                return classDeclaration();
            } else if (match({TokenType::Def})) {
                return functionDeclaration("function", FunctionType::Function, false);
            } else if (match({TokenType::Var})) {
                return varDeclaration();
            } else {
                return statement();
            }
        } catch (std::runtime_error &e) {
            synchronize();
            return nullptr;
        }
    }

    Ref <Statement> Venus::classDeclaration() {
        Token name = consume(TokenType::Identifier, "Expect class name.");
        consume(TokenType::LeftParen, "Expect '(' after class name.");

        Ref<VariableExpression> superClass;

        if (check(TokenType::Identifier)) {
            consume(TokenType::Identifier, "Expect superclass name or ')'.");
            superClass = std::make_shared<VariableExpression>(previous());
        }

        consume(TokenType::RightParen, "Expect right paren after superclass name.");
        consume(TokenType::LeftBrace, "Expect '{' before class body.");

        std::vector<Ref<Statement>> methods = std::vector<Ref<Statement>>();

        while (!check(TokenType::RightBrace) && !isAtEnd()) {
            if (match({TokenType::NewLine})) {
                continue;
            }

            methods.push_back(functionDeclaration("method", FunctionType::Method, false));
        }

        consume(TokenType::RightBrace, "Expect '}' after class body.");

        return std::make_shared<ClassStatement>(name, superClass, methods);
    }

    Ref <Statement> Venus::functionDeclaration(const char *kind, FunctionType type, bool isStatic) {
        Token name = consume(TokenType::Identifier, "Expect " + std::string(kind) + " name.");
        consume(TokenType::LeftParen, "Expect '(' after " + std::string(kind) + " name.");
        std::vector<Token> params = {};

        if (type == FunctionType::Method) {
            consume(TokenType::Self, "Expected 'self' as the first argument of the method.");
        }

        if (!check(TokenType::RightParen)) {
            do {
                if (params.size() >= 255) {
                    error("Can't have more than 255 parameters.");
                }

                params.push_back(consume(TokenType::Identifier, "Expect parameter name."));
            } while (match({TokenType::Comma}));
        }

        consume(TokenType::RightParen, "Expect ')' after parameters.");
        consume(TokenType::LeftBrace, "Expect '{' before " + std::string(kind) + " body.");

        std::vector<Ref<Statement>> body = block();

        return std::make_shared<FunctionStatement>(name, params, body);
    }

    Ref <Statement> Venus::varDeclaration() {
        Token name = consume(TokenType::Identifier, "Expect variable name.");

        Ref<Expression> initializer = nullptr;

        if (match({TokenType::Equal})) {
            initializer = expression();
        }

        consume(TokenType::NewLine, "Expect new line after variable declaration.");

        return std::make_shared<VariableStatement>(name, initializer);
    }

    Ref <Statement> Venus::statement() {
        if (match({TokenType::While})) return whileStatement();
        if (match({TokenType::For})) return forStatement();
        if (match({TokenType::If})) return ifStatement();
        if (match({TokenType::Return})) return returnStatement();
        if (match({TokenType::LeftBrace})) return std::make_shared<BlockStatement>(block());

        return expressionStatement();
    }

    std::vector<Ref < Statement>>

    Venus::block() {
        std::vector<Ref<Statement>> statements = std::vector<Ref<Statement>>();

        while (!check(TokenType::RightBrace) && !isAtEnd()) {
            if (peek().type == TokenType::NewLine) {
                advance();
                continue;
            }
            statements.push_back(declarationStatement());
        }

        consume(TokenType::RightBrace, "Expect '}' after block.");
        return statements;
    }

    Ref <Statement> Venus::forStatement() {
        Ref<Statement> initializer;

        if (match({TokenType::IgnoredVariable})) {
            initializer = nullptr;
        } else if (match({TokenType::Var})) {
            initializer = initializerDeclaration();
        } else {
            initializer = expressionStatement();
        }

        Ref<Expression> condition;

        if (!check(TokenType::Semicolon) && initializer) {
            condition = expression();
        }

        consume(TokenType::Semicolon, "Expect ';' after loop first member.");

        Ref<Expression> increment;

        if (!check(TokenType::Semicolon) && initializer) {
            condition = expression();
            consume(TokenType::Semicolon, "Expect ';' after loop condition.");
        } else {
            consume(TokenType::Semicolon, "Expect ';' after loop first member with '_' marker.");
        }


        if (!check(TokenType::LeftBrace)) {
            consume(TokenType::LeftBrace, "Expect loop body after increment.");
        }

        Ref<Statement> body = statement();

        std::vector<Ref<Statement>> statements = {body, std::make_shared<ExpressionStatement>(increment)};

        if (increment) {
            body = std::make_shared<BlockStatement>(statements);
        }

        if (!condition) {
            condition = std::make_shared<LiteralExpression<bool>>(true);
        }

        body = std::make_shared<WhileStatement>(condition, body);

        if (initializer) {
            std::vector<Ref<Statement>> bodyStatements = {initializer, body};
            body = std::make_shared<BlockStatement>(bodyStatements);
        }

        return body;
    }

    Ref <Statement> Venus::ifStatement() {
        Ref<Expression> condition = expression();
        Ref<Statement> thenBranch = statement();
        Ref<Statement> elseBranch;

        if (match({TokenType::Else})) {
            elseBranch = statement();
        }

        return std::make_shared<IfStatement>(condition, thenBranch, elseBranch);
    }

    Ref <Statement> Venus::whileStatement() {
        Ref<Expression> condition = expression();
        Ref<Statement> body = statement();

        return std::make_shared<WhileStatement>(condition, body);
    }

    Ref <Statement> Venus::returnStatement() {
        Token keyword = previous();
        Ref<Expression> value;

        if (!check(TokenType::NewLine)) {
            value = expression();
        }

        consume(TokenType::NewLine, "Expect new line after return value.");
        return std::make_shared<ReturnStatement>(keyword, value);
    }

    Ref <Statement> Venus::initializerDeclaration() {
        Token name = consume(TokenType::Identifier, "Expect variable name.");

        Ref<Expression> initializer = nullptr;

        if (match({TokenType::Equal})) {
            initializer = expression();
        }

        consume(TokenType::Semicolon, "Expect ';' after initializer declaration.");

        return std::make_shared<VariableStatement>(name, initializer);
    }
}
