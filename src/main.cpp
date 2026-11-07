
  #include <chrono>
  #include <cstdlib>
  #include <exception>
  #include <iostream>
  #include <string>
  #include <thread>
  #include "Game.hpp"
  #include "Level.hpp"
  #include "Menu.hpp"
  #include "Score.hpp"

  namespace {

  std::string levelName(const std::string &path) {
      std::size_t pos = path.find_last_of("/\\");
      return pos == std::string::npos ? path : path.substr(pos + 1);
  }

  void printHelp(const char *program) {
      std::cout << "Sokoban - push every box onto a target.\n\n"
                << "Usage:\n"
                << "  " << program << "                  start with the level menu (levels/levels.txt)\n"
                << "  " << program << " <level> [...]    play the given level files in order\n"
                << "  " << program << " --help           show this help\n\n"
                << "Controls (no Enter needed):\n"
                << "  w/a/s/d  move      u  undo\n"
                << "  r        restart   q  quit\n\n"
                << "Level files: # wall, space floor, . target, $ box,\n"
                << "             * box on target, @ player, + player on target\n"
                << "Best scores (fewest moves) are saved in scores.txt\n";
  }

  // plays one level; returns true if it was solved, false if the player quit
  bool playLevel(const std::string &path, Score &scores) {
      Level level(path);
      Game game(level);
      game.run();
      if (!game.isSolved()) {
          return false;
      }

      std::string name = levelName(path);
      int old = scores.best(name);
      int moves = game.getMoves();
      if (scores.update(name, moves)) {
          scores.save();
          std::cout << "New record: " << moves << " moves\r\n";
      } else {
          std::cout << "Moves: " << moves << ", best: " << old << "\r\n";
      }
      std::cout << std::flush;
      return true;
  }

  }

  int main(int argc, char *argv[]) {
      if (argc >= 2 && std::string(argv[1]) == "--help") {
          printHelp(argv[0]);
          return 0;
      }

      try {
          Score scores("scores.txt");
          std::system("stty raw");

          if (argc >= 2) {
              for (int i = 1; i < argc; ++i) {
                  if (!playLevel(argv[i], scores)) {
                      break;
                  }
                  if (i + 1 < argc) {
                      std::this_thread::sleep_for(std::chrono::milliseconds(2500));
                  } else {
                      std::cout << "All levels completed!\r\n" << std::flush;
                  }
              }
          } else {
              Menu menu("levels/levels.txt");
              int choice = menu.choose(scores);
              while (choice >= 0) {
                  if (playLevel(menu.path(choice), scores)) {
                      std::this_thread::sleep_for(std::chrono::milliseconds(2500));
                  }
                  choice = menu.choose(scores);
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