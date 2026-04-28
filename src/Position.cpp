#include "Position.h"
#include <iostream>


std::ostream& operator<< (std::ostream& os, const Position& pos) { // piece.position in loc de piece.position.col/piece.position.row
    os << "(" << pos.row << ", " << pos.col << ")";
    return os;
}
bool Position::operator==(const Position& other) const {
    return this->row == other.row && this->col == other.col;
}