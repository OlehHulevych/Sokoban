#pragma once
#include <set>
#include <string>

#include "Position.hpp"


class Level {
public:
    explicit Level(const std::string &path);
    bool isWall(Position p) const;
    bool isTarget(Position p) const;
    bool hasBox(Position p) const;
    Position getPlayer() const;
    void moveBox(Position from, Position to);
    void setPlayer(Position p);
    bool isSolved() const;
    int getRows() const;
    int getCols() const;

private:
    std::set<Position> walls_, targets_, boxes_;
    Position player_{};
    int rows_ = 0, cols_ = 0;



};
