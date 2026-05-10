#pragma once
#include "Piece.h"

class RookMovement : virtual public Piece {
public:
    RookMovement(Color c);
    virtual ~RookMovement();

    std::vector<Position> doGetValidMoves(Board &board, Position pos, bool checkCastling) override;
};
