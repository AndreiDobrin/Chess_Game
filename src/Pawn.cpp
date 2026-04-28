//
// Created by Andrei on 4/26/2026.
//

#include "Pawn.h"
#include "Board.h"
#include <iostream>
using namespace std;


void Pawn::printPiece() {
    if (this->getColor() == Color::WHITE)
        std::cout<< "[ wP ]";
    else {
        std::cout << "[ bP ]";
    }
}
std::string Pawn::identifyPiece() {
    return "Pawn";
}

std::vector<Position> Pawn::doGetValidMoves(Board& board, Position pos, bool checkCastling) {
    std::vector<Position> validMoves;
    int r = pos.row;
    int c = pos.col;
    Color pieceColor = board.getPositionInfo(pos)->getColor();
    int moveOrientation = 1;
    if (this->getColor() == Color::WHITE) {
        moveOrientation = -1;
    }
    // are liber in fata un spatiu
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 &&
    board.getPositionInfo({r + moveOrientation, c}) == nullptr) {
        validMoves.push_back({r + moveOrientation, c});
    }
    // are liber in fata doua spatii ( + nu s-a miscat)
    if (!this->getHasMoved() && r + moveOrientation * 2 >= 0 && r + moveOrientation * 2 <= 7 &&
    board.getPositionInfo({r + moveOrientation, c}) == nullptr && // primul spatiu
    board.getPositionInfo({r + moveOrientation * 2, c}) == nullptr) // al doilea spatiu
    {
        validMoves.push_back({r + moveOrientation * 2, c});
    }

    // are piesa pe diagonala stanga SI ARE CULOARE DIFERITA
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 && c - 1 >= 0 &&
    board.getPositionInfo({r + moveOrientation, c - 1}) != nullptr &&
    board.getPositionInfo({r + moveOrientation, c - 1})->getColor() != pieceColor) {
        validMoves.push_back({r + moveOrientation, c - 1});
    }

    // are piesa pe diagonala dreapta SI ARE CULOARE DIFERITA
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 && c + 1 >= 0 &&
    board.getPositionInfo({r + moveOrientation, c + 1}) != nullptr &&
    board.getPositionInfo({r + moveOrientation, c + 1})->getColor() != pieceColor) {
        validMoves.push_back({r + moveOrientation, c + 1});
    }

    // en passant
    if (board.getLastMoveFrom().row != -1 && board.getLastMoveFrom().col != -1) {
        Position lastFrom = board.getLastMoveFrom();
        Position lastTo = board.getLastMoveTo();
        Piece* lastPiece = board.getPositionInfo(lastTo);

        if (lastPiece != nullptr &&
            lastPiece->getColor() != this->getColor() &&
            lastPiece->identifyPiece() == "Pawn") {

            if (abs(lastFrom.row - lastTo.row) == 2) {

                if (pos.row == lastTo.row && abs(pos.col - lastTo.col) == 1) {

                    validMoves.push_back({pos.row + moveOrientation, lastTo.col});
                }
            }
            }
    }

    return validMoves;
}