#include "../../include/frontend/AST.hpp"

namespace paracl::frontend
{

void INode::accept(NodeVisitor& visitor) {
    visitor.visit(this);
}

AST::AST(const AST& rhs) {
    NodeCopier copier{*this};
    root_ = copier.copy(rhs.root_);
}

void AST::dump(std::ostream& os) const {
    NodeDumper dumper{os};
    dumper.dump(root_);
}

void AST::semanticAnalyze(Driver& ) {

}

} // namespace paracl::frontend
