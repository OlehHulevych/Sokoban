#include "catch.hpp"
#include "../src/Game.hpp"
#include "../src/Level.hpp"
#include <string>
#include "catch.hpp"

namespace {
    Level loadLevel1() {
        return Level(std::string(LEVELS_DIR) + "/level1.txt");
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