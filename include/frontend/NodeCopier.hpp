#pragma once

#include "NodeVisitor.hpp"

namespace paracl::frontend
{

class NodeCopier : public NodeVisitor
{
private:
    AST& tree_;
    INode* cur_ = nullptr;

public:
    NodeCopier(AST& tree)
    : tree_(tree) {}

    INode* copy(INode* node) {
        NodeVisitor::visit(node);
        return cur_;
    }

protected:
    void visit(Expression* node) override {
        cur_ = tree_.createNode<Expression>(node->loc_);
    }

    void visit(UnaryExpression* node) override {
        visit(node->expr_);
        Expression* expr = static_cast<Expression*>(cur_);
        cur_ = tree_.createNode<UnaryExpression>(node->loc_, node->op_, expr);
    }

    void visit(BinaryExpression* node) override {
        visit(node->left_);
        Expression* left = static_cast<Expression*>(cur_);
        visit(node->right_);
        Expression* right = static_cast<Expression*>(cur_);
        cur_ = tree_.createNode<BinaryExpression>(node->loc_, node->op_, left, right);
    }

    void visit(TernaryExpression* node) override {
        visit(node->condition_);
        Expression* condition = static_cast<Expression*>(cur_);
        visit(node->onTrue_);
        Expression* onTrue = static_cast<Expression*>(cur_);
        visit(node->onFalse_);
        Expression* onFalse = static_cast<Expression*>(cur_);
        cur_ = tree_.createNode<TernaryExpression>(node->loc_, condition, onTrue, onFalse);
    }

    void visit(ConstantExpression* node) override {
        cur_ = tree_.createNode<ConstantExpression>(node->loc_, node->value_);
    }

    void visit(VariableExpression* node) override {
        cur_ = tree_.createNode<VariableExpression>(node->loc_, node->name_);
    }

    void visit(InputExpression* node) override {
        cur_ = tree_.createNode<InputExpression>(node->loc_);
    }

    void visit(Statement* node) override {
        cur_ = tree_.createNode<Statement>(node->loc_);
    }

    void visit(BlockStatement* node) override {
        BlockStatement* block = tree_.createNode<BlockStatement>(node->loc_);
        for (const auto& statement: node->statements_) {
            visit(statement);
            block->statements_.push_back(static_cast<Statement*>(cur_));
        }
        cur_ = block;
    }

    void visit(ExpressionStatement* node) override {
        visit(node->expr_);
        Expression* expr = static_cast<Expression*>(cur_);
        cur_ = tree_.createNode<ExpressionStatement>(node->loc_, expr);
    }

    void visit(IfStatement* node) override {
        visit(node->condition_);
        Expression* condition = static_cast<Expression*>(cur_);
        visit(node->trueBlock_);
        Statement* trueBlock = static_cast<Statement*>(cur_);
        cur_ = tree_.createNode<IfStatement>(node->loc_, condition, trueBlock);
    }

    void visit(IfElseStatement* node) override {
        visit(node->condition_);
        Expression* condition = static_cast<Expression*>(cur_);
        visit(node->trueBlock_);
        Statement* trueBlock = static_cast<Statement*>(cur_);
        visit(node->falseBlock_);
        Statement* falseBlock = static_cast<Statement*>(cur_);
        cur_ = tree_.createNode<IfElseStatement>(node->loc_, condition, trueBlock, falseBlock);
    }

    void visit(WhileStatement* node) override {
        visit(node->condition_);
        Expression* condition = static_cast<Expression*>(cur_);
        visit(node->block_);
        Statement* block = static_cast<Statement*>(cur_);
        cur_ = tree_.createNode<WhileStatement>(node->loc_, condition, block);
    }

    void visit(OutputStatement* node) override {
        visit(node->expr_);
        Expression* expr = static_cast<Expression*>(cur_);
        cur_ = tree_.createNode<OutputStatement>(node->loc_, expr);
    }

    void visit(BreakStatement* node) override {
        cur_ = tree_.createNode<BreakStatement>(node->loc_);
    }

    void visit(ContinueStatement* node) override {
        cur_ = tree_.createNode<ContinueStatement>(node->loc_);
    }
};

} // namespace paracl::frontend {
