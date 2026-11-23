#include "catch.hpp"
#include "../src/Game.hpp"
#include "../src/Level.hpp"
#include <string>
#include "catch.hpp"

namespace {
    Level loadLevel1() {
        return Level(std::string(LEVELS_DIR) + "/level1.txt");
    }
    Level loadLevel2() {
        return Level(std::string(LEVELS_DIR) + "/level2.txt");
    }
    void play(Game &game, const std::string &keys) {
        for (char c : keys) {
            game.handleCommand(c);
        }
    }

}

TEST_CASE("Game: simple moves") {
    Game game(loadLevel1());

    SECTION("moving onto free floor succeeds") {
        REQUIRE(game.tryMove({0, 1}));
        REQUIRE(game.getMoves() == 1);
        REQUIRE(game.getLevel().getPlayer() == Position{2, 5});
    }

    SECTION("moving into a wall fails and changes nothing") {
        REQUIRE(game.tryMove({-1, 0}));
        REQUIRE_FALSE(game.tryMove({-1, 0}));
        REQUIRE(game.getMoves() == 1);
        REQUIRE(game.getLevel().getPlayer() == Position{1, 4});
    }
}

TEST_CASE("Game: pushing boxes") {
    Game game(loadLevel1());

    SECTION("pushing the box onto the target solves the level") {
        REQUIRE_FALSE(game.isSolved());
        REQUIRE(game.tryMove({0, -1}));
        REQUIRE(game.getLevel().hasBox({2, 2}));
        REQUIRE(game.isSolved());
        REQUIRE(game.getMoves() == 1);
    }

    SECTION("a box cannot be pushed into a wall") {
        REQUIRE(game.tryMove({0, -1}));  
        REQUIRE(game.tryMove({0, -1}));
        REQUIRE_FALSE(game.tryMove({0, -1}));
        REQUIRE(game.getLevel().hasBox({2, 1}));
        REQUIRE(game.getMoves() == 2);
    }


}

TEST_CASE("Game: key commands") {
      Game game(loadLevel1());

      SECTION("w/a/s/d move the player") {
          play(game, "d");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 5});
          play(game, "s");
          REQUIRE(game.getLevel().getPlayer() == Position{3, 5});
          play(game, "a");
          REQUIRE(game.getLevel().getPlayer() == Position{3, 4});
          play(game, "w");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 4);
      }

      SECTION("pushing with 'a' solves the first level") {
          play(game, "a");
          REQUIRE(game.isSolved());
          REQUIRE(game.getMoves() == 1);
      }

      SECTION("unknown keys do nothing") {
          play(game, "xyz5 !");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }

      SECTION("moves into a wall are not counted") {
          play(game, "ww");   // the second 'w' hits the wall
          REQUIRE(game.getLevel().getPlayer() == Position{1, 4});
          REQUIRE(game.getMoves() == 1);
      }
  }

  TEST_CASE("Game: undo") {
      Game game(loadLevel1());

      SECTION("undo takes back a move") {
          play(game, "du");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }

      SECTION("undo takes back a push, including the box") {
          play(game, "a");
          REQUIRE(game.isSolved());
          play(game, "u");
          REQUIRE_FALSE(game.isSolved());
          REQUIRE(game.getLevel().hasBox({2, 3}));
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }

      SECTION("undo can be repeated") {
          play(game, "dsuu");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }

      SECTION("undo with an empty history does nothing") {
          play(game, "u");
          play(game, "uuu");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }

      SECTION("a refused move leaves no undo entry") {
          play(game, "wwu");   // 'w' ok, 'w' into the wall, then one undo
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }
  }

  TEST_CASE("Game: restart") {
      Game game(loadLevel1());

      SECTION("restart returns to the start") {
          play(game, "dsr");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }

      SECTION("restart also restores pushed boxes") {
          play(game, "ar");
          REQUIRE_FALSE(game.isSolved());
          REQUIRE(game.getLevel().hasBox({2, 3}));
      }

      SECTION("restart clears the undo history") {
          play(game, "dsru");
          REQUIRE(game.getLevel().getPlayer() == Position{2, 4});
          REQUIRE(game.getMoves() == 0);
      }
  }

  TEST_CASE("Game: solving level2.txt with a key sequence") {
      Game game(loadLevel2());

      REQUIRE_FALSE(game.isSolved());
      play(game, "asawwdwawdddssdwdss");
      REQUIRE(game.isSolved());
      REQUIRE(game.getMoves() == 19);
  }

  TEST_CASE("Game: a wrong sequence does not solve level2.txt") {
      Game game(loadLevel2());

      play(game, "asawwdwawdddssdwd");   // the last push is missing
      REQUIRE_FALSE(game.isSolved());
  }