//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_STATEMENT_H
#define LUNALANG_STATEMENT_H

#include <utility>

#include "Expression.h"

namespace Luna::Internal::Venus {

    class Statement {
    public:
        Statement() = default;

        virtual ~Statement() = default;

        [[nodiscard]] virtual std::string toString() = 0;
    };

    class ExpressionStatement : public Statement {
    public:
        explicit ExpressionStatement(std::shared_ptr<Expression> expression) : expression(std::move(expression)) {}

        std::string toString() override;

        std::shared_ptr<Expression> expression;
    };

    class FunctionStatement : public Statement {
    public:
        FunctionStatement(const Token &name, const std::vector<Token> &params,
                          const std::vector<std::shared_ptr<Statement>> &body);

        std::string toString() override;

        Token name;
        std::vector<Token> params;
        std::vector<std::shared_ptr<Statement>> body;
    };

    class ClassStatement : public Statement {
    public:

        ClassStatement(Token &name, std::shared_ptr<VariableExpression> &superClass,
                       std::vector<std::shared_ptr<Statement>> &methods) : name(name), superClass(superClass),
                                                                           methods(methods) {}

        std::string toString() override;

        Token name;
        std::shared_ptr<VariableExpression> superClass;
        std::vector<std::shared_ptr<Statement>> methods;
    };

    class VariableStatement : public Statement {
    public:
        VariableStatement(const Token &name, const std::shared_ptr<Expression> &initializer);

        std::string toString() override;

        Token name;
        std::shared_ptr<Expression> initializer;
    };

    class BlockStatement : public Statement {
    public:
        explicit BlockStatement(const std::vector<std::shared_ptr<Statement>> &statements);

        std::string toString() override;

        std::vector<std::shared_ptr<Statement>> statements;
    };

    class WhileStatement : public Statement {
    public:
        WhileStatement(const std::shared_ptr<Expression> &condition, const std::shared_ptr<Statement> &body);

        std::string toString() override;

        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> body;
    };

    class IfStatement : public Statement {
    public:
        IfStatement(const std::shared_ptr<Expression> &condition, const std::shared_ptr<Statement> &thenBranch,
                    const std::shared_ptr<Statement> &elseBranch);

        std::string toString() override;

        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> thenBranch;
        std::shared_ptr<Statement> elseBranch;
    };

    class ReturnStatement : public Statement {
    public:
        ReturnStatement(const Token &keyword, const std::shared_ptr<Expression> &value);

        std::string toString() override;

        Token keyword;
        std::shared_ptr<Expression> value;
    };

    template<typename R>
    class StatementVisitor {
    public:
        virtual R visitBlockStmt(BlockStatement stmt) = 0;

        virtual R visitClassStmt(ClassStatement stmt) = 0;

        virtual R visitExpressionStmt(ExpressionStatement stmt) = 0;

        virtual R visitIfStmt(IfStatement stmt) = 0;

        virtual R visitFunctionStmt(FunctionStatement stmt) = 0;

        virtual R visitReturnStmt(ReturnStatement stmt) = 0;

        virtual R visitVarStmt(VariableStatement stmt) = 0;

        virtual R visitAssignStmt(AssignmentExpression stmt) = 0;

        virtual R visitWhileStmt(WhileStatement stmt) = 0;
    };

} // Venus
// Internal
// Luna

#endif //LUNALANG_STATEMENT_H
