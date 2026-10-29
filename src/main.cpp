#include <iostream>
#include <exception>

#include "Level.hpp"
#include "Game.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <level file>\n";
        return 1;
    }

    try {
        Level level(argv[1]);
        Game game(level);
        std::system("stty raw");
        game.run();
        std::system("stty -raw");
    } catch (const std::exception &e) {
        std::system("stty raw");
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
