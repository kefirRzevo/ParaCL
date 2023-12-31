#pragma once

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>

#include "INode.hpp"

namespace paracl::frontend
{

class Driver;

class AST final
{
private:
    std::vector<std::unique_ptr<INode>> nodes_;
    INode* root_ = nullptr;

public:
    AST() = default;

    AST(const AST& rhs);

    AST& operator=(const AST& rhs) {
        if (this == std::addressof(rhs)) {
            return *this;
        }
        AST temp{rhs};
        *this = std::move(temp);
        return *this;
    }

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

    void setRoot(INode* root) {
        root_ = root;
    }

    INode* getRoot() {
        return root_;
    }

    void clear() {
        nodes_.clear();
        root_ = nullptr;
    }

    void dump(std::ostream& os) const;

    void semanticAnalyze(Driver& driver);
};

} // namespace paracl::frontend {
