#include "include/frontend/Driver.hpp"

using namespace paracl::frontend;

int main(int argc, char* argv[]) {
    try {
        Driver drv{"../tests/ex1.cl"};
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
}