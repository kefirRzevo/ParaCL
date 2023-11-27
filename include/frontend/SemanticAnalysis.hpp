#pragma once

#include "Driver.hpp"
#include "SymTable.hpp"

namespace paracl::frontend
{

struct VariableExpression;
class SymTable;
class ScopeChecker;

class SemanticAnalyzer final
{
private:
    ScopeChecker scopes_;

public:
    SemanticAnalyzer() {}

    void clear() {
        scopes_.clear();
    }

    //void analyze()
};

} //namespace paracl::frontend
