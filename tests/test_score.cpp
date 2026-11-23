//
// Created by Oleh on 11/23/2026.
//

#include "catch.hpp"
#include "Score.hpp"
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>

  namespace {


  struct TempScoreFile {
      std::string path;
      explicit TempScoreFile(const std::string &name) : path(name) {
          std::remove(path.c_str());
      }
      ~TempScoreFile() {
          std::remove(path.c_str());
      }
  };

  void writeFile(const std::string &path, const std::string &content) {
      std::ofstream file(path, std::ios::binary);
      file << content;
  }

  std::string readFile(const std::string &path) {
      std::ifstream file(path, std::ios::binary);
      return std::string(std::istreambuf_iterator<char>(file),
                         std::istreambuf_iterator<char>());
  }

  }

  TEST_CASE("Score: no file means an empty table") {
      TempScoreFile file("tmp_scores_missing.txt");
      Score scores(file.path);

      REQUIRE(scores.best("level1.txt") == -1);
  }

  TEST_CASE("Score: update rules") {
      TempScoreFile file("tmp_scores_update.txt");
      Score scores(file.path);

      SECTION("the first score is always a record") {
          REQUIRE(scores.update("level1.txt", 25));
          REQUIRE(scores.best("level1.txt") == 25);
      }

      SECTION("a worse score is ignored") {
          scores.update("level1.txt", 25);
          REQUIRE_FALSE(scores.update("level1.txt", 30));
          REQUIRE(scores.best("level1.txt") == 25);
      }

      SECTION("a tie is not a record") {
          scores.update("level1.txt", 25);
          REQUIRE_FALSE(scores.update("level1.txt", 25));
          REQUIRE(scores.best("level1.txt") == 25);
      }

      SECTION("a better score replaces the old one") {
          scores.update("level1.txt", 25);
          REQUIRE(scores.update("level1.txt", 20));
          REQUIRE(scores.best("level1.txt") == 20);
      }

      SECTION("levels are independent") {
          scores.update("level1.txt", 25);
          scores.update("level2.txt", 40);
          REQUIRE(scores.best("level1.txt") == 25);
          REQUIRE(scores.best("level2.txt") == 40);
          REQUIRE(scores.best("level3.txt") == -1);
      }
  }

  TEST_CASE("Score: saving and loading") {
      TempScoreFile file("tmp_scores_saveload.txt");

      SECTION("a saved table can be loaded again") {
          {
              Score scores(file.path);
              scores.update("level1.txt", 25);
              scores.update("level2.txt", 40);
              scores.save();
          }
          Score loaded(file.path);
          REQUIRE(loaded.best("level1.txt") == 25);
          REQUIRE(loaded.best("level2.txt") == 40);
      }

      SECTION("the file has one 'name moves' line per level, sorted by name") {
          Score scores(file.path);
          scores.update("b.txt", 20);
          scores.update("a.txt", 10);
          scores.save();
          REQUIRE(readFile(file.path) == "a.txt 10\nb.txt 20\n");
      }

      SECTION("a score that was not saved is lost") {
          {
              Score scores(file.path);
              scores.update("level1.txt", 25);
          }
          Score loaded(file.path);
          REQUIRE(loaded.best("level1.txt") == -1);
      }

      SECTION("an improved score overwrites the file content") {
          {
              Score scores(file.path);
              scores.update("level1.txt", 25);
              scores.save();
          }
          {
              Score scores(file.path);
              REQUIRE(scores.update("level1.txt", 18));
              scores.save();
          }
          Score loaded(file.path);
          REQUIRE(loaded.best("level1.txt") == 18);
          REQUIRE(readFile(file.path) == "level1.txt 18\n");
      }
  }

  TEST_CASE("Score: loading an existing file") {
      TempScoreFile file("tmp_scores_existing.txt");

      SECTION("a valid file") {
          writeFile(file.path, "a.txt 5\nb.txt 7\n");
          Score scores(file.path);
          REQUIRE(scores.best("a.txt") == 5);
          REQUIRE(scores.best("b.txt") == 7);
      }

      SECTION("an empty file") {
          writeFile(file.path, "");
          Score scores(file.path);
          REQUIRE(scores.best("a.txt") == -1);
      }

      SECTION("a damaged line does not crash the program") {
          writeFile(file.path, "a.txt 5\nbroken\nb.txt 7\n");
          Score scores(file.path);
          REQUIRE(scores.best("a.txt") == 5);
      }
  }
