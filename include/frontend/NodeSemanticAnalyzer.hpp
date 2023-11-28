#pragma once

#include "Driver.hpp"
#include "NodeVisitor.hpp"

namespace paracl::frontend
{

class Driver;

struct VariableExpression;
class SymTable;
class ScopeChecker;

class Visitor
{
private:
    
};

class SemanticAnalyzer : public NodeVisitor
{
private:
    Driver& driver_;
    ScopeChecker scopes_;

public:
    SemanticAnalyzer(Driver& driver)
    : driver_(driver) {}

    void clear() {
        scopes_.clear();
    }

    void analyze() {
        const INode* root = driver_.getAST()->getRoot();
    }

protected:
    void visit(Expression* node) override {

    }

    void visit(UnaryExpression* node) override {

    }

    void visit(BinaryExpression* node) override {

    }

    void visit(TernaryExpression* node) override {

    }

    void visit(ConstantExpression* node) override {

    }

    void visit(VariableExpression* node) override {

    }

    void visit(InputExpression* node) override {

    }

    void visit(Statement* node) override {

    }

    void visit(BlockStatement* node) override {

    }

    void visit(ExpressionStatement* node) override {

    }

    void visit(IfStatement* node) override {

    }

    void visit(IfElseStatement* node) override {

    }

    void visit(WhileStatement* node) override {

    }

    void visit(OutputStatement* node) override {

    }

    void visit(BreakStatement* node) override {

    }

    void visit(ContinueStatement* node) override {

    }
};

} //namespace paracl::frontend
