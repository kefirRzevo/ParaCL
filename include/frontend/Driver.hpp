#pragma once

#include "parser.tab.hh"
#include "SymTable.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "AST.hpp"

namespace paracl::frontend
{

class Driver
{
private:
    std::string filepath_;

    std::unique_ptr<AST> tree_ = nullptr;
    std::unique_ptr<Lexer> lexer_ = nullptr;
    std::unique_ptr<Parser> parser_ = nullptr;
    std::unique_ptr<ScopeChecker> scopes_ = nullptr;
    std::unique_ptr<ErrorReporter> reporter_ = nullptr;

    void initializeLexerParser() {
        lexer_ = std::make_unique<Lexer>(this, std::addressof(filepath_));
        parser_ = std::make_unique<Parser>(*lexer_, *this);
    }

    void clearLexerParser() {
        lexer_.reset();
        parser_.reset();
    }

public:
    Driver(const std::string& filepath)
    : filepath_(filepath) {
        tree_ = std::make_unique<AST>();
        scopes_ = std::make_unique<ScopeChecker>();
        reporter_ = std::make_unique<ErrorReporter>();
        initializeLexerParser();
    }

    void setFile(const std::string& filepath) {
        filepath_ = filepath;
        clearLexerParser();
        initializeLexerParser();
        tree_->clear();
        scopes_->clear();
        reporter_->clear();
    }

    void parse() {
        parser_->parse();
    }

    template<typename NodeType, typename... NodeArgs>
    NodeType* createNode(NodeArgs&&... args) {
        return tree_->createNode<NodeType>(std::forward<NodeArgs>(args)...);
    }

    void setRoot(Node* root) {
        tree_->setRoot(root);
    }

    const Node* getRoot() const {
        return tree_->getRoot();
    }

    void dump(std::ostream& os) const {
        return tree_->dump(os);
    }

    template<typename ErrorType, typename... ErrorArgs>
    void reportError(ErrorArgs&&... args) {
        reporter_->reportError<ErrorType>(std::forward<ErrorArgs>(args)...);
    }

    bool hasErrors() const {
        return reporter_->hasErrors();
    }

    void reportAllErrors(std::ostream& os) const {
        reporter_->reportAllErrors(os);
    }
};

} // namespace paracl::frontend
