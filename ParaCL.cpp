#include <fstream>
#include "include/frontend/Driver.hpp"

using namespace paracl::frontend;

int main() {
    try {
        Driver drv{"../tests/ex1.cl"};
        drv.getParser()->parse();
        if (drv.getAST()->getRoot() != nullptr) {
            std::fstream dumpFile{"../res/dump_ex1.dot", std::ios::out};
            drv.getAST()->dump(dumpFile);
        }
        if (drv.getReporter()->hasErrors()) {
            drv.getReporter()->reportAllErrors(std::cout);
        } else {
            assert(drv.getAST()->getRoot());
            std::cout << "Nice" << std::endl;
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}
