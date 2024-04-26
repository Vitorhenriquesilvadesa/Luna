//
// Created by vitor on 24/04/24.
//

#include "Statement.h"


namespace Luna::Internal::Venus {
    std::string ExpressionStatement::toString() {
        return this->expression->toString();
    }

    std::string ClassStatement::toString() {
        return "Class Statement.";
    }

    std::string FunctionStatement::toString() {
        return "Function Statement.";
    }

    FunctionStatement::FunctionStatement(const Token &name, const std::vector<Token> &params,
                                         const std::vector<std::shared_ptr<Statement>> &body) : name(name),
                                                                                                params(params),
                                                                                                body(body) {}

    VariableStatement::VariableStatement(const Token &name, const std::shared_ptr<Expression> &initializer) : name(
            name), initializer(initializer) {}

    std::string VariableStatement::toString() {
        return "Variable Statement.";
    }

    BlockStatement::BlockStatement(const std::vector<std::shared_ptr<Statement>> &statements) : statements(
            statements) {}

    std::string BlockStatement::toString() {
        return "Block Statement.";
    }

    WhileStatement::WhileStatement(const std::shared_ptr<Expression> &condition, const std::shared_ptr<Statement> &body)
            : condition(condition), body(body) {}

    std::string WhileStatement::toString() {
        return "While Statement.";
    }

    IfStatement::IfStatement(const std::shared_ptr<Expression> &condition, const std::shared_ptr<Statement> &thenBranch,
                             const std::shared_ptr<Statement> &elseBranch) : condition(condition),
                                                                             thenBranch(thenBranch),
                                                                             elseBranch(elseBranch) {}

    std::string IfStatement::toString() {
        return "If Statement.";
    }

    ReturnStatement::ReturnStatement(const Token &keyword, const std::shared_ptr<Expression> &value) : keyword(keyword),
                                                                                                       value(value) {}

    std::string ReturnStatement::toString() {
        return "Return Statement.";
    }
} // Venus
// Internal
// Luna