//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_EXPRESSION_H
#define LUNALANG_EXPRESSION_H


#include "../Base/LunaBaseType.h"
#include "../Mercury/Token.h"

#include <memory>
#include <utility>
#include <string>
#include <vector>

using namespace Luna::Internal::Mercury;
using namespace Luna::Internal::Base;

namespace Luna::Internal::Venus {

    class Expression {
    public:
        Expression() = default;

        [[nodiscard]] virtual std::string toString() const = 0;

        virtual ~Expression() = default;
    };

    class BinaryExpression final : public Expression {
    public:

        BinaryExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right, const Token &exprOperator)
                : left(std::move(left)), right(std::move(right)),
                  exprOperator(exprOperator) {}

        [[nodiscard]] std::string toString() const override;

        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
        Token exprOperator;
    };

    class CallExpression final : public Expression {
    public:
        CallExpression(const std::shared_ptr<Expression> &callee, const Token &paren,
                       const std::vector<std::shared_ptr<Expression>> &arguments);

        [[nodiscard]] std::string toString() const override;

        std::shared_ptr<Expression> callee;
        Token paren;
        std::vector<std::shared_ptr<Expression>> arguments;
    };

    class LogicalExpression final : public Expression {
    public:

        LogicalExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right,
                          const Token &exprOperator)
                : left(std::move(left)), right(std::move(right)),
                  exprOperator(exprOperator) {}

        [[nodiscard]] std::string toString() const override;

        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
        Token exprOperator;
    };

    class TernaryExpression final : public Expression {
    public:
        TernaryExpression(const std::shared_ptr<Expression> &condition, const std::shared_ptr<Expression> &left,
                          const std::shared_ptr<Expression> &right);

        [[nodiscard]] std::string toString() const override;

        std::shared_ptr<Expression> condition;
        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
    };

    class VariableExpression final : public Expression {

    public:
        explicit VariableExpression(const Token &name);

        [[nodiscard]] std::string toString() const override;

        Token name;
    };

    class AssignmentExpression final : public Expression {
    public:
        AssignmentExpression(const Token &name, const std::shared_ptr<Expression> &value);

        [[nodiscard]] std::string toString() const override;

        Token name;
        std::shared_ptr<Expression> value;
    };

    class GetExpression final : public Expression {

    public:
        GetExpression(const std::shared_ptr<Expression> &object, const Token &name);

        [[nodiscard]] std::string toString() const override;

        std::shared_ptr<Expression> object;
        Token name;
    };

    class SetExpression final : public Expression {

    public:
        SetExpression(const Token &name, const std::shared_ptr<Expression> &object,
                      const std::shared_ptr<Expression> &value);

        [[nodiscard]] std::string toString() const override;

        Token name;
        std::shared_ptr<Expression> object;
        std::shared_ptr<Expression> value;

    };

    class UnaryExpression final : public Expression {
    public:
        UnaryExpression(const Token &exprOperator, const std::shared_ptr<Expression> &right);

        [[nodiscard]] std::string toString() const override;

        Token exprOperator;
        std::shared_ptr<Expression> right;
    };

    class GroupingExpression final : public Expression {
    public:
        explicit GroupingExpression(const std::shared_ptr<Expression> &expr);

        [[nodiscard]] std::string toString() const override;

        std::shared_ptr<Expression> expr;
    };

    class SuperExpression final : public Expression {
    public:
        explicit SuperExpression(const Token &keyword, const Token &method) : keyword(keyword), method(method) {}

        [[nodiscard]] std::string toString() const override;

        Token keyword;
        Token method;
    };

    class SelfExpression final : public Expression {
    public:
        explicit SelfExpression(const Token &keyword) : keyword(keyword) {}

        [[nodiscard]] std::string toString() const override;

        Token keyword;
    };

    class BreakExpression final : public Expression {
    public:
        explicit BreakExpression() {}

        [[nodiscard]] std::string toString() const override;
    };

    template<typename T>
    class LiteralExpression final : public Expression {
    public:
        explicit LiteralExpression(T value) : value(Object<T>(value)) {}

        [[nodiscard]] std::string toString() const override;

        Object <T> value;
    };

    template<typename T>
    std::string LiteralExpression<T>::toString() const {
        return "Literal expression";
    }

    template<typename T>
    class ExpressionVisitor {
    public:
        virtual T visitAssignExpr(AssignmentExpression expr) = 0;

        virtual T visitBinaryExpr(BinaryExpression expr) = 0;

        virtual T visitTernaryExpr(TernaryExpression expr) = 0;

        virtual T visitCallExpr(CallExpression expr) = 0;

        virtual T visitGetExpr(GetExpression expr) = 0;

        virtual T visitGroupingExpr(GroupingExpression expr) = 0;

        virtual T visitLiteralExpr(LiteralExpression<void *> expr) = 0;

        virtual T visitLogicalExpr(LogicalExpression expr) = 0;

        virtual T visitSetExpr(SetExpression expr) = 0;

        virtual T visitSuperExpr(SuperExpression expr) = 0;

        virtual T visitThisExpr(SelfExpression expr) = 0;

        virtual T visitUnaryExpr(UnaryExpression expr) = 0;

        virtual T visitVariableExpr(VariableExpression expr) = 0;
    };
} // Venus
// Internal
// Luna

#endif //LUNALANG_EXPRESSION_H
