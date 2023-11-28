#pragma once

#include "AST.hpp"

namespace paracl::frontend
{

class NodeVisitor
{
public:
    virtual void visit(INode* node) {
        if (typeid(*node) == typeid(Expression)) {
            visit(static_cast<Expression*>(node));
        } else if (typeid(*node) == typeid(UnaryExpression)) {
            visit(static_cast<UnaryExpression*>(node));
        } else if (typeid(*node) == typeid(BinaryExpression)) {
            visit(static_cast<BinaryExpression*>(node));
        } else if (typeid(*node) == typeid(TernaryExpression)) {
            visit(static_cast<TernaryExpression*>(node));
        } else if (typeid(*node) == typeid(ConstantExpression)) {
            visit(static_cast<ConstantExpression*>(node));
        } else if (typeid(*node) == typeid(VariableExpression)) {
            visit(static_cast<VariableExpression*>(node));
        } else if (typeid(*node) == typeid(InputExpression)) {
            visit(static_cast<InputExpression*>(node));
        } else if (typeid(*node) == typeid(Statement)) {
            visit(static_cast<Statement*>(node));
        } else if (typeid(*node) == typeid(BlockStatement)) {
            visit(static_cast<BlockStatement*>(node));
        } else if (typeid(*node) == typeid(ExpressionStatement)) {
            visit(static_cast<ExpressionStatement*>(node));
        } else if (typeid(*node) == typeid(IfStatement)) {
            visit(static_cast<IfStatement*>(node));
        } else if (typeid(*node) == typeid(IfElseStatement)) {
            visit(static_cast<IfElseStatement*>(node));
        } else if (typeid(*node) == typeid(WhileStatement)) {
            visit(static_cast<WhileStatement*>(node));
        } else if (typeid(*node) == typeid(OutputStatement)) {
            visit(static_cast<OutputStatement*>(node));
        } else if (typeid(*node) == typeid(BreakStatement)) {
            visit(static_cast<BreakStatement*>(node));
        } else if (typeid(*node) == typeid(ContinueStatement)) {
            visit(static_cast<ContinueStatement*>(node));
        } else {
            throw std::runtime_error("Can not cast base class");
        }
    }

protected:
    virtual void visit(Expression* node) = 0;

    virtual void visit(UnaryExpression* node) = 0;

    virtual void visit(BinaryExpression* node) = 0;

    virtual void visit(TernaryExpression* node) = 0;

    virtual void visit(ConstantExpression* node) = 0;

    virtual void visit(VariableExpression* node) = 0;

    virtual void visit(InputExpression* node) = 0;

    virtual void visit(Statement* node) = 0;

    virtual void visit(BlockStatement* node) = 0;

    virtual void visit(ExpressionStatement* node) = 0;

    virtual void visit(IfStatement* node) = 0;

    virtual void visit(IfElseStatement* node) = 0;

    virtual void visit(WhileStatement* node) = 0;

    virtual void visit(OutputStatement* node) = 0;

    virtual void visit(BreakStatement* node) = 0;

    virtual void visit(ContinueStatement* node) = 0;
};

} // namespace paracl::frontend
