#include <iostream>
#include <exception>
#include <thread>
#include <cstdlib>
#include <chrono>
#include "Level.hpp"
#include "Game.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <level file>\n";
        return 1;
    }

    try {
       std::system("stty raw");
        for (int i=1;i<argc;++i) {
            Level level(argv[i]);
            Game game(level);
            game.run();
            if (!game.isSolved()) {
                break;
            }

            if (i+1<argc) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            }
            else {
                std::cout << "All levels completed!\r\n" << std::flush;
            }

        }
        std::system("stty -raw");
    } catch (const std::exception &e) {
        std::system("stty -raw");
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
