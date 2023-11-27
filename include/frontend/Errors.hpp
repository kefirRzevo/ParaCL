#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "location.hpp"

namespace paracl::frontend
{

struct Error
{
    location loc_;
    std::string msg_;

    Error(const location& loc, const std::string& msg) :
        loc_(loc), msg_(msg) {}

    virtual void print(std::ostream& os) const {
        os << loc_ << " error '" << msg_ << "'" << std::endl;
    }

    virtual ~Error() = default;
};

struct Lexical : public Error
{
    using Error::loc_;
    using Error::msg_;

    Lexical(const location& loc, const std::string& msg) :
        Error(loc, msg) {}

    void print(std::ostream& os) const override {
        os << loc_ << " error: lexical '" << msg_ << "'" << std::endl;
    }
};

struct UnknownToken : public Lexical
{
    using Lexical::loc_;
    using Lexical::msg_;

    std::string token_;

    UnknownToken(const location& loc, const std::string& token) :
        Lexical(loc, "unknown type name "), token_(token) {}

    void print(std::ostream& os) const override {
        os << loc_ << " error: " << msg_ << "'" << token_ << "'" << std::endl;
    }
};

struct UnterminatedComment : public Lexical
{
    using Lexical::loc_;
    using Lexical::msg_;

    UnterminatedComment(const location& loc) :
        Lexical(loc, "unterminated /* comment") {}

    void print(std::ostream& os) const override {
        os << loc_ << " error: " << msg_ << std::endl;
    }
};

struct Syntax : public Error
{
    using Error::loc_;
    using Error::msg_;

    Syntax(const location& loc, const std::string& msg) :
        Error(loc, msg) {}

    void print(std::ostream& os) const override {
        os << loc_ << " " << msg_ << std::endl;
    }
};

struct Semantic : public Error
{
    using Error::loc_;
    using Error::msg_;

    Semantic(const location& loc, const std::string& msg)
    : Error(loc, msg) {}

    void print(std::ostream& os) const override {
        os << loc_ << " error: semantic '" << msg_ << "'" << std::endl;
    }
};

struct ErrorAttachments
{

};

class ErrorReporter
{
private:
    std::vector<std::unique_ptr<Error>> errors_;

public:
    ErrorReporter() {}

    template<typename ErrorType, typename... ErrorArgs>
    void reportError(ErrorArgs&&... args) {
        auto uptr = std::make_unique<ErrorType>(std::forward<ErrorArgs>(args)...);
        errors_.push_back(std::move(uptr));
    }

    Error* lastError() {
        return errors_.empty() ? nullptr : errors_.back().get();
    }

    bool hasErrors() const {
        return errors_.size();
    }

    void reportAllErrors(std::ostream& os) const {
        for (const auto& error: errors_) {
            error->print(os);
        }
    }

    void clear() {
        errors_.clear();
    }
};

} // namespace paracl::frontend
