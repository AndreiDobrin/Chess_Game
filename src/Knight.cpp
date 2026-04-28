//
// Created by Andrei on 4/26/2026.
//

#include "Knight.h"
#include <iostream>
using namespace std;


void Knight::printPiece() {
    if (this->getColor() == Color::WHITE)
        std::cout << "[ wN ]";
    else
        std::cout << "[ bN ]";
}

std::vector<Position> Knight::doGetValidMoves(Board& board, Position pos, bool checkCastling) {
    std::vector<Position> validMoves;

    int r = pos.row;
    int c = pos.col;

    //dreapta sus
    // _ _ _
    // |
    int r1 = r - 1;
    int c1 = c + 2;
    if (r1 >= 0 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //stanga sus
    // _ _ _
    //     |
    r1 = r - 1;
    c1 = c - 2;
    if (r1 >= 0 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //dreapta jos
    // |
    // _ _ _
    r1 = r + 1;
    c1 = c + 2;
    if (r1 <= 7 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //stanga jos
    //     |
    // _ _ _
    r1 = r + 1;
    c1 = c - 2;
    if (r1 <= 7 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    // _ _
    // |
    // |
    r1 = r - 2;
    c1 = c + 1;
    if (r1 >= 0 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    // _ _
    //   |
    //   |
    r1 = r - 2;
    c1 = c - 1;
    if (r1 >= 0 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    // |
    // |
    // _ _
    r1 = r + 2;
    c1 = c + 1;
    if (r1 <= 7 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //   |
    //   |
    // _ _
    r1 = r + 2;
    c1 = c - 1;
    if (r1 <= 7 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    return validMoves;
}
std::string Knight::identifyPiece() {
    return "Knight";
}