//
// Created by vitor on 25/04/24.
//

#ifndef LUNALANG_EARTH_H
#define LUNALANG_EARTH_H

#include "../Venus/Expression.h"
#include "../Venus/Statement.h"

using namespace Luna::Internal::Venus;

namespace Luna::Internal::Earth {

    class Earth : public ExpressionVisitor<void*>, StatementVisitor<void> {
    public:
        void* visitAssignExpr(AssignmentExpression expr);
        void* visitBinaryExpr(BinaryExpression expr);
        void* visitTernaryExpr(TernaryExpression expr);
        void* visitCallExpr(CallExpression expr);
        void* visitGetExpr(GetExpression expr);
        void* visitGroupingExpr(GroupingExpression expr);
        void* visitLiteralExpr(LiteralExpression<void *> expr);
        void* visitLogicalExpr(LogicalExpression expr);
        void* visitSetExpr(SetExpression expr);
        void* visitSuperExpr(SuperExpression expr);
        void* visitThisExpr(SelfExpression expr);
        void* visitUnaryExpr(UnaryExpression expr);
        void* visitVariableExpr(VariableExpression expr);
        void visitBlockStmt(BlockStatement stmt);
        void visitClassStmt(ClassStatement stmt);
        void visitExpressionStmt(ExpressionStatement stmt);
        void visitIfStmt(IfStatement stmt);
        void visitFunctionStmt(FunctionStatement stmt);
        void visitReturnStmt(ReturnStatement stmt);
        void visitVarStmt(VariableStatement stmt);
        void visitAssignStmt(AssignmentExpression stmt);
        void visitWhileStmt(WhileStatement stmt);
    };
}

#endif //LUNALANG_EARTH_H
