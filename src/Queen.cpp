//
// Created by Andrei on 4/26/2026.
//

#include "Queen.h"
#include <iostream>
#include "Board.h"
using namespace std;

void Queen::printPiece() {
    if (this->getColor() == Color::WHITE)
        std::cout << "[ wQ ]";
    else
        std::cout << "[ bQ ]";
}

void Queen::printPieceConsole() {
    printPiece();
}

std::string Queen::identifyPiece() {
    return "Queen";
}

std::vector<Position> Queen::doGetValidMoves(Board &board, Position pos, bool checkCastling) {
    std::vector<Position> validMoves;
    
    // combinam miscarile (Diamond Inheritance demonstration)
    std::vector<Position> diagonalMoves = BishopMovement::doGetValidMoves(board, pos, checkCastling);
    std::vector<Position> straightMoves = RookMovement::doGetValidMoves(board, pos, checkCastling);
    
    validMoves.insert(validMoves.end(), diagonalMoves.begin(), diagonalMoves.end());
    validMoves.insert(validMoves.end(), straightMoves.begin(), straightMoves.end());
    
    return validMoves;
}
