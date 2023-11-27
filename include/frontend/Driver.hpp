#pragma once

#include "parser.tab.hh"
#include "SemanticAnalysis.hpp"
#include "SymTable.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "AST.hpp"

namespace paracl::frontend
{

class AST;
class Lexer;
class Parser;
class ErrorReposter;
class SematicAnalyzer;

class Driver
{
private:
    std::string filepath_ = "";

    std::unique_ptr<AST> tree_ = nullptr;
    std::unique_ptr<Lexer> lexer_ = nullptr;
    std::unique_ptr<Parser> parser_ = nullptr;
    std::unique_ptr<ErrorReporter> reporter_ = nullptr;
    std::unique_ptr<SemanticAnalyzer> analyzer_ = nullptr;

public:
    Driver(const std::string& filepath)
    : filepath_(filepath) {
        tree_ = std::make_unique<AST>();
        lexer_ = std::make_unique<Lexer>(*this, getFilepath());
        parser_ = std::make_unique<Parser>(*this);
        analyzer_ = std::make_unique<SemanticAnalyzer>();
        reporter_ = std::make_unique<ErrorReporter>();
    }

    void setFile(const std::string& filepath) {
        filepath_ = filepath;
        lexer_.reset();
        lexer_ = std::make_unique<Lexer>(*this, getFilepath());
        tree_->clear();
        reporter_->clear();
        analyzer_->clear();
    }

    std::string* getFilepath() {
        return std::addressof(filepath_);
    }

    AST* getAST() {
        return tree_.get();
    }

    Lexer* getLexer() {
        return lexer_.get();
    }

    Parser* getParser() {
        return parser_.get();
    }

    ErrorReporter* getReporter() {
        return reporter_.get();
    }

    SemanticAnalyzer* getAnalyzer() {
        return analyzer_.get();
    }
};

} // namespace paracl::frontend
