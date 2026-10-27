
#include "Level.hpp"
#include <fstream>
#include <stdexcept>

Level::Level(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    bool foundPlayer = false;
    std::string line;
    int row = 0;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        for (int col = 0; col < static_cast<int>(line.size()); col++) {
            Position p {row, col};
            char c = line[col];
            if (c=='#') walls_.insert(p);
            if (c=='.') targets_.insert(p);
            if (c == '$' || c == '*')boxes_.insert(p);
            if (c=='@' || c=='+') {
                player_ = p;
                foundPlayer = true;
            }
        }
        if (static_cast<int>(line.size()) > 2) cols_ = static_cast<int>(line.size());
        ++row;

    }
    rows_ = row;
    if (!foundPlayer) {
        throw std::runtime_error("Level has no player: " + path);
    }
    if (boxes_.size() != targets_.size()) {
        throw std::runtime_error("Boxes and targets count differ: " + path);
    }
}

bool Level::isWall(Position p) const {
    return walls_.count(p) > 0;
}

bool Level::isTarget(Position p) const {
    return targets_.count(p) > 0;
}

bool Level::hasBox(Position p) const {
    return boxes_.count(p) > 0;
}

Position Level::getPlayer() const {
    return player_;
}

void Level::moveBox(Position from, Position to) {
    boxes_.erase(from);
    boxes_.insert(to);
}

void Level::setPlayer(Position p) {
    player_ = p;
}

bool Level::isSolved() const {
    for (const Position &b : boxes_) {
        if (targets_.count(b) == 0) return false;
    }
    return true;
}


