#pragma once
#include "Piece.h"

class BishopMovement : virtual public Piece {
public:
    BishopMovement(Color c);
    virtual ~BishopMovement();

    std::vector<Position> doGetValidMoves(Board &board, Position pos, bool checkCastling) override;
};
