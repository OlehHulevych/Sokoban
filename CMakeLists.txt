cmake_minimum_required(VERSION 4.3)
project(Sokoban CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON )

add_executable(Sokoban
        src/main.cpp
        src/Level.cpp
        src/Game.cpp
        src/Renderer.cpp
        src/Position.h
)
target_compile_options(-Wall -Wextra -pedantic)
