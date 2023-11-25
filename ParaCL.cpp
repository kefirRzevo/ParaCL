#include <fstream>
#include "include/frontend/Driver.hpp"

using namespace paracl::frontend;

int main() {
    try {
        Driver drv{"../tests/ex1.cl"};
        drv.parse();
        std::fstream dumpFile{"../res/dump_ex1.dot", std::ios::out};
        drv.dump(dumpFile);
        std::cout << "Nice" << std::endl;
        if (drv.hasErrors()) {
            assert(0);
            drv.reportAllErrors(std::cout);
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
}