#pragma once

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include "location.hpp"
#include "SymTable.hpp"

namespace paracl::frontend
{

struct Node
{
    location loc_;

    Node(const location &loc) :
        loc_(loc) {}

    virtual void dump(std::ostream& os) const;

    virtual ~Node() {}
};

struct Expression : public Node
{
    using Node::loc_;

    Expression(const location& loc) :
        Node(loc) {}

    void dump(std::ostream& os) const override;
};

enum class UnaryOperation
{
    UN_SUB,
    UN_ADD,
    UN_NOT,

    UN_PREFIX_INC,
    UN_PREFIX_DEC,
    UN_POSTFIX_INC,
    UN_POSTFIX_DEC,
};

struct UnaryExpression : public Expression
{
    using Expression::loc_;

    UnaryOperation op_;
    Expression* expr_ = nullptr;

    UnaryExpression(const location& loc, UnaryOperation op, Expression* expr) :
        Expression(loc), op_(op), expr_(expr) {}

    void dump(std::ostream& os) const override;
};

enum class BinaryOperation
{
    BIN_MUL,
    BIN_DIV,
    BIN_MOD,

    BIN_ADD,
    BIN_SUB,

    BIN_L,
    BIN_G,
    BIN_LE,
    BIN_GE,

    BIN_EQ,
    BIN_NE,
    BIN_AND,
    BIN_OR,

    BIN_ASSIGN,
    BIN_COMMA,
};

struct BinaryExpression : public Expression
{
    using Expression::loc_;

    BinaryOperation op_;
    Expression* left_ = nullptr;
    Expression* right_ = nullptr;

    BinaryExpression(const location& loc, BinaryOperation op, Expression* left, Expression* right) :
        Expression(loc), op_(op), left_(left), right_(right) {}

    void dump(std::ostream& os) const override;
};

struct TernaryExpression : public Expression
{
    using Expression::loc_;

    Expression* condition_ = nullptr;
    Expression* onTrue_ = nullptr;
    Expression* onFalse_ = nullptr;

    TernaryExpression(const location& loc, Expression* condition, Expression* onTrue, Expression* onFalse) :
        Expression(loc), condition_(condition), onTrue_(onTrue), onFalse_(onFalse) {}

    void dump(std::ostream& os) const override;
};

struct ConstantExpression : public Expression
{
    using Expression::loc_;

    int value_;

    ConstantExpression(const location& loc, const int& value) :
        Expression(loc), value_(value) {}

    void dump(std::ostream& os) const override;
};

struct VariableExpression : public Expression
{
    using Expression::loc_;

    std::string name_;

    VariableExpression(const location& loc, const std::string& name) :
        Expression(loc), name_(name) {}

    void dump(std::ostream& os) const override;
};

struct InputExpression : public Expression
{
    using Expression::loc_;

    InputExpression(const location& loc):
        Expression(loc) {}

    void dump(std::ostream& os) const override;
};

struct ErrorExpression : public Expression
{
    std::string msg_;

    ErrorExpression(const location& loc, const std::string& msg) :
        Expression(loc), msg_(msg) {}

    void dump(std::ostream& os) const override;
};

struct Statement : public Node
{
    using Node::loc_;

    Statement(const location &loc) :
        Node(loc) {}

    void dump(std::ostream& os) const override;
};

struct BlockStatement : public Statement
{
    using Statement::loc_;

    SymTable table_;
    std::vector<Statement* > statements_;

    BlockStatement(const location& loc) :
        Statement(loc) {}

    void dump(std::ostream& os) const override;
};

struct ExpressionStatement : public Statement
{
    using Statement::loc_;

    Expression* expr_;

    ExpressionStatement(const location& loc, Expression* expr) :
        Statement(loc), expr_(expr) {}

    void dump(std::ostream& os) const override;
};

struct IfStatement : public Statement
{
    using Statement::loc_;

    Expression* condition_ = nullptr;
    Statement* trueBlock_ = nullptr;
    Statement* falseBlock_ = nullptr;

    IfStatement(const location& loc, Expression* condition, Statement* trueBlock) :
        Statement(loc), condition_(condition), trueBlock_(trueBlock) {}

    IfStatement(const location& loc, Expression* condition, Statement* trueBlock, Statement* falseBlock) :
        Statement(loc), condition_(condition), trueBlock_(trueBlock), falseBlock_(falseBlock) {}

    void dump(std::ostream& os) const override;
};

struct WhileStatement : public Statement
{
    using Statement::loc_;

    Expression* condition_ = nullptr;
    Statement* block_ = nullptr;

    WhileStatement(const location& loc, Expression* condition, Statement* block) :
        Statement(loc), condition_(condition), block_(block) {}

    void dump(std::ostream& os) const override;
};

struct OutputStatement : public Statement
{
    using Statement::loc_;

    Expression* expr_;

    OutputStatement(const location& loc, Expression* expr) :
        Statement(loc), expr_(expr) {}

    void dump(std::ostream& os) const override;
};

struct BreakStatement : public Statement
{
    using Statement::loc_;

    BreakStatement(const location& loc) :
        Statement(loc) {}

    void dump(std::ostream& os) const override;
};

struct ContinueStatement : public Statement
{
    using Statement::loc_;

    ContinueStatement(const location& loc) :
        Statement(loc) {}

    void dump(std::ostream& os) const override;
};

class AST final
{
private:
    std::vector<std::unique_ptr<Node>> nodes_;
    Node* root_ = nullptr;

public:
    AST() = default;
    AST(const AST&) = delete;
    AST& operator=(const AST&) = delete;
    AST(AST&&) = default;
    AST& operator=(AST&&) = default;
    ~AST() = default;

    template<typename NodeType, typename... NodeArgs>
    NodeType* createNode(NodeArgs&&... args) {
        auto uptr = std::make_unique<NodeType>(std::forward<NodeArgs>(args)...);
        auto ptr = uptr.get();
        nodes_.push_back(std::move(uptr));
        return ptr;
    }

    void setRoot(Node* root) {
        root_ = root;
    }

    const Node* getRoot() const {
        return root_;
    }

    void clear() {
        nodes_.clear();
        root_ = nullptr;
    }

    void dump(std::ostream& os) const {
        os << "digraph {\n";
        os << "\tnode[shape=record, style=filled, fontcolor=black];\n";
        root_->dump(os);
        os << "}" << std::endl;
    }
};

} // namespace paracl::frontend {
