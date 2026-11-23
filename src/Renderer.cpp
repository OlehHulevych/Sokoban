#include "Renderer.hpp"
#include <iostream>

namespace {

    const char *const RESET  = "\033[0m";
    const char *const BLUE   = "\033[34m";
    const char *const YELLOW = "\033[33m";
    const char *const GREEN  = "\033[32m";
    const char *const RED    = "\033[31m";
    const char *const PLAYER = "\033[1;36m";

    void put(char symbol, const char *color) {
        std::cout << color << symbol << RESET;
    }

}

void Renderer::draw(const Level &level) const {
    for (int row = 0; row < level.getRows(); ++row) {
        for (int col = 0; col < level.getCols(); ++col) {
            drawCell(level, Position{row, col});
        }
        std::cout << "\r\n";
    }
}


void Renderer::drawCell(const Level &level, Position p) const {
    if (level.isWall(p)) {
        put('#', BLUE);
        return;
    }
    if (level.hasBox(p)) {
        if (level.isTarget(p)) {
            put('*', GREEN);
        } else {
            put('$', YELLOW);
        }
        return;
    }
    if (p == level.getPlayer()) {
        put(level.isTarget(p) ? '+' : '@', PLAYER);
        return;
    }
    if (level.isTarget(p)) {
        put('.', RED);
        return;
    }
    std::cout << ' ';
}
