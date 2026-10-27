#include <iostream>
#include "Level.hpp"
#include "Renderer.hpp"
#include <exception>

int main(int argc, char* argv[]) {
    if (argc<2) {
        std:: cerr<< "Usage: " << argv[0] << " <level file> \n";
        return 1;
    }

    try {
        Level level(argv[1]);
        Renderer renderer;
        renderer.draw(level);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
