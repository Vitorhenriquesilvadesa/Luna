//
// Created by vitor on 24/04/24.
//

#include "Expression.h"

namespace Luna::Internal::Venus {

    VariableExpression::VariableExpression(const Token &name) : name(name) {}

    std::string VariableExpression::toString() const {
        return this->name.lexeme;
    }

    AssignmentExpression::AssignmentExpression(const Token &name, const std::shared_ptr<Expression> &value) : name(
            name), value(value) {}

    std::string AssignmentExpression::toString() const {
        return this->name.lexeme + " " + this->value->toString();
    }

    GetExpression::GetExpression(const std::shared_ptr<Expression> &object, const Token &name) : object(object),
                                                                                                 name(name) {}

    std::string GetExpression::toString() const {
        return this->object->toString();
    }

    SetExpression::SetExpression(const Token &name, const std::shared_ptr<Expression> &object,
                                 const std::shared_ptr<Expression> &value) : name(name), object(object), value(value) {}

    std::string SetExpression::toString() const {
        return this->name.toString();
    }

    UnaryExpression::UnaryExpression(const Token &exprOperator, const std::shared_ptr<Expression> &right)
            : exprOperator(exprOperator), right(right) {}

    std::string UnaryExpression::toString() const {
        return "Unary Expression";
    }

    std::string BinaryExpression::toString() const {
        return this->left->toString() + " " + tokenStringMap.at(this->exprOperator.type) + " " +
               this->right->toString();
    }

    GroupingExpression::GroupingExpression(const std::shared_ptr<Expression> &expr) : expr(expr) {}

    std::string GroupingExpression::toString() const {
        return this->expr->toString();
    }

    TernaryExpression::TernaryExpression(const std::shared_ptr<Expression> &condition,
                                         const std::shared_ptr<Expression> &left,
                                         const std::shared_ptr<Expression> &right) : condition(condition), left(left),
                                                                                     right(right) {}

    std::string TernaryExpression::toString() const {
        return "Ternary Expression";
    }

    std::string LogicalExpression::toString() const {
        return "Logical Expression";
    }

    CallExpression::CallExpression(const std::shared_ptr<Expression> &callee, const Token &paren,
                                   const std::vector<std::shared_ptr<Expression>> &arguments) : callee(callee),
                                                                                                paren(paren),
                                                                                                arguments(arguments) {}

    std::string CallExpression::toString() const {
        return "Call Expression";
    }

    std::string SuperExpression::toString() const {
        return "Super Expression";
    }

    std::string SelfExpression::toString() const {
        return "Self Expression";
    }

    std::string BreakExpression::toString() const {
        return "Break Expression";
    }
}