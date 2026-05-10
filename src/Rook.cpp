//
// Created by Andrei on 4/26/2026.
//

#include "Rook.h"
#include <iostream>
using namespace std;

    void Rook::printPiece() {
        if (this-> getColor() == Color::WHITE)
            std::cout << "[ wR ]";
        else {
            std::cout << "[ bR ]";
        }
        }

        void Rook::printPieceConsole() {
        printPiece();
        }
        std::vector<Position> Rook::doGetValidMoves(Board& board, Position pos, bool checkCastling) {
        // extindem comportamentul din RookMovement
        return RookMovement::doGetValidMoves(board, pos, checkCastling);
    }
    
    std::string Rook::identifyPiece() {
        return "Rook";
    }
