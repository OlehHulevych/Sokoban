#include <iostream>

#include "Level.hpp"

int main(int argc, char* argv[]) {
    if (argc<2) {
        std:: cerr<< "Usage: " << argv[0] << " <level file> \n";
        return 1;
    }

    try {
        Level level(argv[1]);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
