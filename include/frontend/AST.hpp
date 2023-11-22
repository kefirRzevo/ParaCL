#pragma once

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
//#include "../../res/location.hpp"

struct point
{
    int line;
    int column;
};

struct location
{
    point begin;
    point end;
};

namespace paracl::frontend
{

struct Node
{
    location loc_;

    Node(const location &loc) :
        loc_(loc) {}

    virtual void dump(std::ostream& os) const = 0;

    virtual ~Node() {}
};

struct ErrorNode : public Node
{
    std::string msg_;

    ErrorNode(const std::string& msg, const location& loc) :
        Node(loc), msg_(msg) {}
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

    UnaryExpression op_;
    Expression* expr_ = nullptr;

    UnaryExpression(UnaryOperation op, Expression* expr, const location& loc) :
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

    BinaryExpression(BinaryOperation op, Expression* left, Expression* right, const location& loc) :
        Expression(loc), op_(op), left_(left), right_(right) {}

    void dump(std::ostream& os) const override;
};

struct TernaryExpression : public Expression
{
    using Expression::loc_;

    Expression* condition_ = nullptr;
    Expression* trueExpr_ = nullptr;
    Expression* falseExpr_ = nullptr;

    BinaryExpression(Expression* condition, Expression* trueExpr, Expression* falseExpr, const location& loc) :
        Expression(loc), condition_(condition), trueExpr_(trueExpr), falseExpr_(falseExpr) {}

    void dump(std::ostream& os) const override;
};

struct ConstantExpression : public Expression
{
    using Expression::loc_;

    int value_;

    ConstantExpression(const int& value, const location& loc) :
        Expression(loc), value_(value) {}

    void dump(std::ostream& os) const override;
};

struct VariableExpression : public Expression
{
    using Expression::loc_;

    std::string name_;

    VariableExpression(const std::string& name, const location& loc) :
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

struct AssignmentStatement : public Statement
{
    using Statement::loc_;

    VariableExpression* left_;
    Expression* right_;

    AssignmentStatement(VariableExpression* left, Expression* right, const location& loc) :
        Statement(loc), left_(left), right_(right) {}

    void dump(std::ostream& os) const override;
};

struct IfStatement : public Statement
{
    using Statement::loc_;

    Expression* condition_ = nullptr;
    BlockStatement* trueBlock_ = nullptr;
    BlockStatement* falseBlock_ = nullptr;

    IfStatement(Expression* condition, BlockStatement* trueBlock, const location& loc) :
        Statement(loc), condition_(condition), trueBlock_(trueBlock) {}

    IfStatement(Expression* condition, BlockStatement* trueBlock, BlockStatement* falseBlock, const location& loc) :
        Statement(loc), condition_(condition), trueBlock_(trueBlock), falseBlock_(falseBlock) {}

    void dump(std::ostream& os) const override;
};

struct WhileStatement : public Statement
{
    using Statement::loc_;

    Expression* condition_ = nullptr;
    BlockStatement* block_ = nullptr;

    WhileStatement(Expression* condition, BlockStatement* block, const location& loc) :
        Statement(loc), condition_(condition), block_(block) {}

    void dump(std::ostream& os) const override;
};

struct OutputStatement : public Statement
{
    using Statement::loc_;

    Expression* expr_;

    OutputStatement(Expression* expr, const location& loc) :
        Statement(loc), expr_(expr) {}

    void dump(std::ostream& os) const override;
};

class AST final
{
private:
    std::vector<std::unique_ptr<Node>> nodes_;
    Node* root_ = nullptr;

public:
    AST() = default;
    AST(const ASTe&) = delete;
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

    void dump(std::ostream& os) const {
        os << "digraph {";
        root_->dump();
        os << "}" << std::endl;
    }
};

} // namespace paracl::frontend {
