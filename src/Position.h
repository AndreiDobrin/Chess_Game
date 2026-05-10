#pragma once
#include <iostream>

struct Position {
    int row;
    int col;

    Position(int r = -1, int c = -1);

    friend std::ostream& operator<< (std::ostream& os, const Position& pos);
    bool operator==(const Position& other) const;
};