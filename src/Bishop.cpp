//
// Created by Andrei on 4/26/2026.
//

#include "Bishop.h"
#include <iostream>
using namespace std;

    void Bishop::printPiece() {
        if (this->getColor() == Color::WHITE)
            std::cout << "[ wB ]";
        else
            std::cout << "[ bB ]";
    }

void Bishop::printPieceConsole() {
    printPiece();
}

    std::vector<Position> Bishop::doGetValidMoves(Board &board, Position pos, bool checkCastling){
        // extindem comportamentul din BishopMovement (cerinta: apelam versiunea bazei)
        return BishopMovement::doGetValidMoves(board, pos, checkCastling);
    }
    
    std::string Bishop::identifyPiece() {
        return "Bishop";
    }
