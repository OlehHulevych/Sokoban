#include "catch.hpp"
  #include "Level.hpp"
  #include <cstdio>
  #include <fstream>
  #include <stdexcept>
  #include <string>

  namespace {

  std::string level1Path() {
      return std::string(LEVELS_DIR) + "/level1.txt";
  }

  struct TempFile {
      std::string path;
      TempFile(const std::string &name, const std::string &content) : path(name) {
          std::ofstream file(path, std::ios::binary);
          file << content;
      }
      ~TempFile() {
          std::remove(path.c_str());
      }
  };

  }

  TEST_CASE("Level: loading level1.txt") {
      Level level(level1Path());

      SECTION("size") {
          REQUIRE(level.getRows() == 5);
          REQUIRE(level.getCols() == 7);
      }

      SECTION("positions of the objects") {
          REQUIRE(level.getPlayer() == Position{2, 4});
          REQUIRE(level.isWall({0, 0}));
          REQUIRE(level.hasBox({2, 3}));
          REQUIRE(level.isTarget({2, 2}));
      }

      SECTION("empty floor is nothing") {
          REQUIRE_FALSE(level.isWall({1, 1}));
          REQUIRE_FALSE(level.hasBox({1, 1}));
          REQUIRE_FALSE(level.isTarget({1, 1}));
      }

      SECTION("cells outside the map are not walls, boxes or targets") {
          REQUIRE_FALSE(level.isWall({-1, -1}));
          REQUIRE_FALSE(level.hasBox({100, 100}));
      }
  }

  TEST_CASE("Level: solved state") {
      Level level(level1Path());

      REQUIRE_FALSE(level.isSolved());
      level.moveBox({2, 3}, {2, 2});
      REQUIRE(level.hasBox({2, 2}));
      REQUIRE_FALSE(level.hasBox({2, 3}));
      REQUIRE(level.isSolved());
  }

  TEST_CASE("Level: player can be moved") {
      Level level(level1Path());
      level.setPlayer({1, 1});
      REQUIRE(level.getPlayer() == Position{1, 1});
  }

  TEST_CASE("Level: combined symbols") {
      TempFile file("tmp_level_symbols.txt",
                    "######\n"
                    "#*+$ #\n"
                    "######\n");
      Level level(file.path);

      SECTION("'*' is a box on a target") {
          REQUIRE(level.hasBox({1, 1}));
          REQUIRE(level.isTarget({1, 1}));
      }

      SECTION("'+' is the player on a target") {
          REQUIRE(level.getPlayer() == Position{1, 2});
          REQUIRE(level.isTarget({1, 2}));
      }

      SECTION("'$' is a box on plain floor") {
          REQUIRE(level.hasBox({1, 3}));
          REQUIRE_FALSE(level.isTarget({1, 3}));
      }

      SECTION("not solved while a box is off its target") {
          REQUIRE_FALSE(level.isSolved());
      }
  }

  TEST_CASE("Level: lines of different length") {
      TempFile file("tmp_level_uneven.txt",
                    "#####\n"
                    "#@$.#\n"
                    "###\n");
      Level level(file.path);

      REQUIRE(level.getRows() == 3);
      REQUIRE(level.getCols() == 5);
  }

  TEST_CASE("Level: Windows line endings") {
      TempFile file("tmp_level_crlf.txt",
                    "#####\r\n"
                    "#@$.#\r\n"
                    "#####\r\n");
      Level level(file.path);

      REQUIRE(level.getCols() == 5);
      REQUIRE(level.isWall({1, 4}));
      REQUIRE(level.isTarget({1, 3}));
  }

  TEST_CASE("Level: invalid files are rejected") {
      SECTION("missing file") {
          REQUIRE_THROWS_AS(Level("this_file_does_not_exist.txt"), std::runtime_error);
      }

      SECTION("no player") {
          TempFile file("tmp_level_noplayer.txt",
                        "####\n"
                        "#$.#\n"
                        "####\n");
          REQUIRE_THROWS_AS(Level(file.path), std::runtime_error);
      }

      SECTION("more boxes than targets") {
          TempFile file("tmp_level_mismatch.txt",
                        "#####\n"
                        "#@$ #\n"
                        "#####\n");
          REQUIRE_THROWS_AS(Level(file.path), std::runtime_error);
      }

      SECTION("empty file") {
          TempFile file("tmp_level_empty.txt", "");
          REQUIRE_THROWS_AS(Level(file.path), std::runtime_error);
      }
  }