#pragma once

#include "../../res/scanner.h"
#include "../../res/parser.h"
#include "SymTable.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "AST.hpp"

class Driver
{
private:
    std::string filepath_;

    std::unique_ptr<AST> tree_;
    std::unique_ptr<Lexer> lexer_;
    std::unique_ptr<ScopeChecker> scopes_;
    std::unique_ptr<ErrorReporter> reporter_;

public:
    Driver(const std::string& filepath):
        filepath_(filepath) {}

    template<typename NodeType, typename... NodeArgs>
    NodeType* createNode(NodeArgs&&... args) {
        return tree_.createNode<NodeType>(std::forward<NodeArgs>(args)...);
    }

    void setRoot(Node* root) {
        tree_.setRoot(root);
    }

    const Node* getRoot() const {
        return tree_.getRoot();
    }

    template<typename ErrorType, typename... ErrorArgs>
    void reportError(NodeArgs&&... args) {
        reporter_.reportError<ErrorType>(std::forward<ErrorArgs>(args)...);
    }
};
