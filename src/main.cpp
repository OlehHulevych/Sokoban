#include <iostream>
#include <exception>
#include <thread>
#include <cstdlib>
#include <chrono>
#include "Level.hpp"
#include "Game.hpp"
#include "Score.hpp"

std::string levelName(const std::string &path) {
    std::size_t pos = path.find_last_of("/\\");
    return pos == std::string::npos ? path : path.substr(pos + 1);
}

int main(int argc, char *argv[]) {
    Score scores("scores.txt");
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
            std::string name = levelName(argv[i]);
            if (!game.isSolved()) {
                break;
            }
            if (scores.update(name, game.getMoves())) {
                scores.save();
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


