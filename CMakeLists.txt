cmake_minimum_required(VERSION 4.2.3)
project(Sokoban CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(sokoban_core STATIC
        src/Level.cpp
        src/Renderer.cpp
        src/Game.cpp
        src/Score.cpp
        src/Menu.cpp)
target_include_directories(sokoban_core PUBLIC src)
target_link_libraries(sokoban_core PUBLIC Threads::Threads)
target_compile_options(sokoban_core PRIVATE -Wall -Wextra -pedantic)

add_executable(Sokoban src/main.cpp)
target_link_libraries(Sokoban PRIVATE sokoban_core)
target_compile_options(Sokoban PRIVATE -Wall -Wextra -pedantic)

add_executable(tests
        tests/test_main.cpp
        tests/test_game.cpp
        tests/test_level.cpp
        tests/test_score.cpp)
target_link_libraries(tests PRIVATE sokoban_core)
target_compile_definitions(tests PRIVATE LEVELS_DIR="${CMAKE_SOURCE_DIR}/levels")