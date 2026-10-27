#pragma once
#include "Level.hpp"

class Renderer {
public:
    void draw(const Level &level) const;

private:
    void drawCell(const Level &level, Position p) const;
};


