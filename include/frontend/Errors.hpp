#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "../../res/location.hpp"

struct Error
{
    location loc_;

    Error(const location& loc) :
        loc_(loc) {}

    virtual void print(std::ostream& os) const = 0;

    virtual ~Error() = default;
};

struct Lexical : public Error
{
    std::string msg_;

    LexicalError(const std::string& msg, const location& loc) :
        Error(loc), msg_(msg) {}
};

struct UnknownToken : public Lexical
{
    std::string token_;

    UnknownToken(const std::string& token, const location& loc) :
        Lexical("unknown type name", loc), token_(token) {}

    void print(std::ostream& os) const override {
        os << ":" << loc_.begin.line << ":" << loc_.begin.column: " error: " << msg_ << "'" << token_ << "'" << std::endl;
    }
};

struct Syntax : public Error
{
    std::string msg_;

    Syntax(const std::string& msg, const location& loc) :
        Error(loc), msg_(msg) {}
};

struct IncompleteExpression : public Syntax
{

};

/*
struct Semantic : public Error
{

};

struct ErrorAttachments
{

};
//*/

class ErrorReporter
{
private:
    std::vector<std::unique_ptr<Error>> errors_;

    struct ErrorCompare
    {
        bool operator()(const unique_ptr<Error>& lhs, const unique_ptr<Error>& rhs) const {
            auto lhsLine = lhs->loc_.begin.line;
            auto rhsLine = rhs->loc_.begin.line;
            auto lhsCol = lhs->loc_.begin.column;
            auto rhsCol = rhs->loc_.begin.column;
            return lhsLine < rhsLine ? true :
                                       lhsCol < rhsCol ? true :
                                                         false;
        }
    }

public:
    ErrorReporter() {}

    template<typename ErrorType, typename... ErrorArgs>
    void reportError(NodeArgs&&... args) {
        auto uptr = std::make_unique<ErrorType>(std::forward<ErrorArgs>(args)...);
        errors_.push_back(std::move(uptr));
    }

    void reportAllErrors(std::ostream& os) const {
        std::sort(errors_.begin(), errors_.end(), ErrorCompare);
        for (const auto& error: errors_) {
            error->print(os);
        }
    }
};
