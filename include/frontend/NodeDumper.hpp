#pragma once

#include <vector>
#include "NodeVisitor.hpp"

namespace paracl::frontend
{

class NodeDumper : public NodeVisitor
{
private:
    using NodeVisitor::visit;
    std::ostream& os_;
    std::vector<INode*> nodes_;

    void printLink(INode* parent, INode* child) {
        os_ << "\tnode_" << parent << " -> node_" << child << ";\n";
    }

    void printNode(INode* node, const std::string& color, const std::string& label) {
        os_ << "\tnode_" << node << "[fillcolor=" << color << ", label = \"" << node->loc_ << " | " << label << "\"];\n";
    }

    void push(INode* node) {
        nodes_.push_back(node);
    }

    INode* pop() {
        INode* node = nullptr;
        if (!nodes_.empty()){
            node = nodes_.back();
            nodes_.pop_back();
        }
        return node;
    }

    std::string unOper(UnaryOperation op) {
        switch (op) {
        case UnaryOperation::UN_SUB:
            return "-";
        case UnaryOperation::UN_ADD:
            return "+";
        case UnaryOperation::UN_NOT:
            return "!";
        case UnaryOperation::UN_PREFIX_INC:
            return "++^";
        case UnaryOperation::UN_PREFIX_DEC:
            return "--^";
        case UnaryOperation::UN_POSTFIX_INC:
            return "^++";
        case UnaryOperation::UN_POSTFIX_DEC:
            return "^--";
        default:
            return "Unknown";
        }
    }

    std::string binOper(BinaryOperation op) {
        switch (op) {
        case BinaryOperation::BIN_MUL:
            return "*";
        case BinaryOperation::BIN_DIV:
            return "/";
        case BinaryOperation::BIN_MOD:
            return "%";
        case BinaryOperation::BIN_ADD:
            return "+";
        case BinaryOperation::BIN_SUB:
            return "-";
        case BinaryOperation::BIN_L:
            return "\\<";
        case BinaryOperation::BIN_G:
            return "\\>";
        case BinaryOperation::BIN_LE:
            return "\\<=";
        case BinaryOperation::BIN_GE:
            return "\\>=";
        case BinaryOperation::BIN_EQ:
            return "==";
        case BinaryOperation::BIN_NE:
            return "!=";
        case BinaryOperation::BIN_AND:
            return "&&";
        case BinaryOperation::BIN_OR:
            return "||";
        case BinaryOperation::BIN_ASSIGN:
            return "=";
        case BinaryOperation::BIN_COMMA:
            return ",";
        default:
            return "Unknown";
        }
    }

public:
    NodeDumper(std::ostream& os)
    : os_(os) {}

    void dump(INode* root) {
        os_ << "digraph {\n";
        os_ << "\tnode[shape=record, style=filled, fontcolor=black];\n";
        push(root);
        while(INode* node = pop()) {
            visit(node);
        }
        os_ << "}" << std::endl;
    }

protected:
    virtual void visit(Expression* node) {
        printNode(node, "orange", "Expression");
    }

    virtual void visit(UnaryExpression* node) {
        push(node->expr_);
        printNode(node, "orange", unOper(node->op_));
        printLink(node, node->expr_);
    }

    virtual void visit(BinaryExpression* node) {
        push(node->left_);
        push(node->right_);
        printNode(node, "orange", binOper(node->op_));
        printLink(node, node->left_);
        printLink(node, node->right_);
    }

    virtual void visit(TernaryExpression* node) {
        push(node->condition_);
        push(node->onTrue_);
        push(node->onFalse_);
        printNode(node, "orange", "? :");
        printLink(node, node->condition_);
        printLink(node, node->onTrue_);
        printLink(node, node->onFalse_);
    }

    virtual void visit(ConstantExpression* node) {
        printNode(node, "yellow", std::to_string(node->value_));
    }

    virtual void visit(VariableExpression* node) {
        printNode(node, "pink", node->name_);
    }

    virtual void visit(InputExpression* node) {
        printNode(node, "orange", "Input");
    }

    virtual void visit(Statement* node) {
        printNode(node, "green", "Statement");
    }

    virtual void visit(BlockStatement* node) {
        printNode(node, "green", "Statement Block");
        for (auto& statement: node->statements_) {
            push(statement);
            printLink(node, statement);
        }
    }

    virtual void visit(ExpressionStatement* node) {
        push(node->expr_);
        printNode(node, "green", "Expression Statement");
        printLink(node, node->expr_);
    }

    virtual void visit(IfStatement* node) {
        push(node->condition_);
        push(node->trueBlock_);
        printNode(node, "green", "if");
        printLink(node, node->condition_);
        printLink(node, node->trueBlock_);
    }

    virtual void visit(IfElseStatement* node) {
        push(node->condition_);
        push(node->trueBlock_);
        push(node->falseBlock_);
        printNode(node, "green", "if else");
        printLink(node, node->condition_);
        printLink(node, node->trueBlock_);
        printLink(node, node->falseBlock_);
    }

    virtual void visit(WhileStatement* node) {
        push(node->condition_);
        push(node->block_);
        printNode(node, "green", "while");
        printLink(node, node->condition_);
        printLink(node, node->block_);
    }

    virtual void visit(OutputStatement* node) {
        push(node->expr_);
        printNode(node, "greenn", "Output");
        printLink(node, node->expr_);
    }

    virtual void visit(BreakStatement* node) {
        printNode(node, "green", "Break");
    }

    virtual void visit(ContinueStatement* node) {
        printNode(node, "green", "Break");
    }
};

} // namespace paracl::frontend {
