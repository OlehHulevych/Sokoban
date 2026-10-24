#pragma once

struct Position{
    int row;
    int col;

    Position operator+(const Position &p) const {
        return Position{row+p.row, col+p.col};
    }
    bool operator<(const Position &p) const {
        if (row!=p.row) return row<p.row;
        return col<p.col;
    }
    bool operator==(const Position &p) const {
        return row == p.row && col == p.col;
    }
};

