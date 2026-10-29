#include "Renderer.hpp"
#include <iostream>

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
        std::cout << '#';
        return;
    }
    if (level.hasBox(p)) {
        std::cout << (level.isTarget(p) ? '*' : '$');
        return;
    }
    if (p == level.getPlayer()) {
        std::cout << (level.isTarget(p) ? '+' : '@');
        return;
    }
    if (level.isTarget(p)) {
        std::cout << '.';
        return;
    }
    std::cout << ' ';
}
