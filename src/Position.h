#pragma once
#include <iostream>

struct Position {
    int row;
    int col;

    friend std::ostream& operator<< (std::ostream& os, const Position& pos);
    bool operator==(const Position& other) const;
};