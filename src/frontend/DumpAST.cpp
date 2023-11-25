#include "../../include/frontend/AST.hpp"

namespace paracl::frontend
{

void Node::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=red];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Abstract Node\"];\n";
}

void ErrorNode::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=red];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Error " << msg_ << "\"];\n";
}

void Expression::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=orange];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Expression\"];\n";
}

void UnaryExpression::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=orange];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | ";
    switch (op_) {
    case UnaryOperation::UN_SUB:
        os << "-";
        break;
    case UnaryOperation::UN_ADD:
        os << "+";
        break;
    case UnaryOperation::UN_NOT:
        os << "!";
        break;
    case UnaryOperation::UN_PREFIX_INC:
        os << "++^";
        break;
    case UnaryOperation::UN_PREFIX_DEC:
        os << "--^";
        break;
    case UnaryOperation::UN_POSTFIX_INC:
        os << "^++";
        break;
    case UnaryOperation::UN_POSTFIX_DEC:
        os << "^--";
        break;
    default:
        os << "Unknown";
        break;
    }
    os << "\"];\n";

    assert(expr_);
    os << "\tnode_" << this << " -> node_" << expr_ << ";\n";
    expr_->dump(os);
}

void BinaryExpression::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=orange];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | ";
    switch (op_) {
    case BinaryOperation::BIN_MUL:
        os << "*";
        break;
    case BinaryOperation::BIN_DIV:
        os << "/";
        break;
    case BinaryOperation::BIN_MOD:
        os << "%";
        break;
    case BinaryOperation::BIN_ADD:
        os << "+";
        break;
    case BinaryOperation::BIN_SUB:
        os << "-";
        break;
    case BinaryOperation::BIN_L:
        os << "<";
        break;
    case BinaryOperation::BIN_G:
        os << ">";
        break;
    case BinaryOperation::BIN_LE:
        os << "<=";
        break;
    case BinaryOperation::BIN_GE:
        os << ">=";
        break;
    case BinaryOperation::BIN_EQ:
        os << "==";
        break;
    case BinaryOperation::BIN_NE:
        os << "!=";
        break;
    case BinaryOperation::BIN_AND:
        os << "&&";
        break;
    case BinaryOperation::BIN_OR:
        os << "||";
        break;
    case BinaryOperation::BIN_ASSIGN:
        os << "=";
        break;
    case BinaryOperation::BIN_COMMA:
        os << ",";
        break;
    default:
        os << "Unknown";
        break;
    }
    os << "\"];\n";

    assert(left_);
    os << "\tnode_" << this << " -> node_" << left_ << ";\n";
    left_->dump(os);

    assert(right_);
    os << "\tnode_" << this << " -> node_" << right_ << ";\n";
    right_->dump(os);
}

void TernaryExpression::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=orange];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | ? :\"];\n";

    assert(condition_);
    os << "\tnode_" << this << " -> node_" << condition_ << ";\n";
    condition_->dump(os);

    assert(onTrue_);
    os << "\tnode_" << this << " -> node_" << onTrue_ << ";\n";
    onTrue_->dump(os);

    assert(onFalse_);
    os << "\tnode_" << this << " -> node_" << onFalse_ << ";\n";
    onFalse_->dump(os);
}

void ConstantExpression::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=yellow];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | " << value_ << "\"];\n";
}

void VariableExpression::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=pink];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | " << name_ << "\"];\n";
}

void InputExpression::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=pink];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Input\"];\n";
}

void Statement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Statement\"];\n";
}

void BlockStatement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Statement Block\"];\n";
    for (const auto& statement: statements_) {
        assert(statement);
        os << "\tnode_" << this << " -> node_" << statement << ";\n";
        statement->dump(os);
    }
}

void ExpressionStatement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Expression Statement\"];\n";

    assert(expr_);
    os << "\tnode_" << this << " -> node_" << expr_ << ";\n";
    expr_->dump(os);
}

void IfStatement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    falseBlock_
    ? (os << "\tnode_" << this << "[label = \"" << loc_ << " | If\"];\n")
    : (os << "\tnode_" << this << "[label = \"" << loc_ << " | If Else\"];\n");

    assert(condition_);
    os << "\tnode_" << this << " -> node_" << condition_ << ";\n";
    condition_->dump(os);

    assert(trueBlock_);
    os << "\tnode_" << this << " -> node_" << trueBlock_ << ";\n";
    trueBlock_->dump(os);

    if (falseBlock_) {
        assert(falseBlock_);
        os << "\tnode_" << this << " -> node_" << falseBlock_ << ";\n";
        falseBlock_->dump(os);
    }
}

void WhileStatement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | While\"];\n";

    assert(condition_);
    os << "\tnode_" << this << " -> node_" << condition_ << ";\n";
    condition_->dump(os);

    assert(block_);
    os << "\tnode_" << this << " -> node_" << block_ << ";\n";
    block_->dump(os);
}

void OutputStatement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Output Statement\"];\n";

    assert(expr_);
    os << "\tnode_" << this << " -> node_" << expr_ << ";\n";
    expr_->dump(os);
}

void BreakStatement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Break\"];\n";
}

void ContinueStatement::dump(std::ostream& os) const {
    os << "\tnode[shape=record, style=filled, fontcolor=black, fillcolor=green];\n";
    os << "\tnode_" << this << "[label = \"" << loc_ << " | Continue\"];\n";
}

} // namespace paracl::frontend {
