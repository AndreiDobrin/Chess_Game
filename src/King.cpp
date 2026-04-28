//
// Created by Andrei on 4/26/2026.
//

#include "King.h"
#include "Piece.h"
#include "Board.h"
#include <iostream>
using namespace std;

void King::printPiece() {
        if (this->getColor() == Color::WHITE)
            std::cout << "[ wK ]";
        else {
            std::cout << "[ bK ]";
        }
    }

// std::vector<Position> King::getLegalMoves(Board& board, std::vector<Position> validMoves, Position pos) {
//
// }

std::vector<Position> King::doGetValidMoves(Board& board, Position pos, bool checkCastling) {
        std::vector<Position> validMoves;
    // int r = pos.row;
    // int c = pos.col;
    Color pieceColor = board.getPositionInfo(pos)->getColor();

    for (int i = pos.row - 1; i <= pos.row + 1; i++) {
        for (int j = pos.col - 1; j <= pos.col + 1; j++) {
            if (i >= 0 && i <= 7 && j >= 0 && j <= 7 &&
            (board.getPositionInfo({i,j}) == nullptr ||
            board.getPositionInfo({i,j})->getColor() != pieceColor))
                validMoves.push_back({i,j});
        }
    }

    // castling
    if (checkCastling && this->getHasMoved() == false && !board.isKingInCheck(pieceColor)) {
        if (pieceColor == Color::WHITE) {
            if (board.getPositionInfo({7,7}) != nullptr) {
                if (board.getPositionInfo({7, 7})->getHasMoved() == false &&
                   board.getPositionInfo({7, 6}) == nullptr &&
                   board.getPositionInfo({7, 5}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {7,5})) {
                        validMoves.push_back({7, 6});
                    }
                   }
            }
            if (board.getPositionInfo({7,0}) != nullptr) {
                if (board.getPositionInfo({7, 0})->getHasMoved() == false &&
                   board.getPositionInfo({7, 1}) == nullptr &&
                   board.getPositionInfo({7, 2}) == nullptr &&
                   board.getPositionInfo({7, 3}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {7,3}) &&
                        !board.isPieceAttacked(Color::WHITE, {7,2})) {

                        validMoves.push_back({7, 2});
                    }
                   }
            }
        }

        if (pieceColor == Color::BLACK) {
            if (board.getPositionInfo({0,0}) != nullptr) {
                if (board.getPositionInfo({0,0})->getHasMoved() == false &&
                   board.getPositionInfo({0,1}) == nullptr &&
                   board.getPositionInfo({0,2}) == nullptr &&
                   board.getPositionInfo({0,3}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {0,3}) &&
                        !board.isPieceAttacked(Color::WHITE, {0,2})) {

                        validMoves.push_back({0,2});
                    }
                   }
            }
            if (board.getPositionInfo({0,7}) != nullptr) {
                if (board.getPositionInfo({0,7})->getHasMoved() == false &&
                   board.getPositionInfo({0,6}) == nullptr &&
                   board.getPositionInfo({0,5}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {0,5})) {
                        validMoves.push_back({0,6});
                    }
                   }
            }
        }
    }

    return validMoves;
}

std::string King::identifyPiece() {
        return "King";
    };

