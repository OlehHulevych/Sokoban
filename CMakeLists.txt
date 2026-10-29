cmake_minimum_required(VERSION 4.2.3)
project(Sokoban CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Sokoban
        src/main.cpp
        src/Level.cpp
        src/Renderer.cpp
        src/Game.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(Sokoban PRIVATE Threads::Threads)

target_compile_options(Sokoban PRIVATE -Wall -Wextra -pedantic)
