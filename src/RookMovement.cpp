#include "RookMovement.h"
#include "Board.h"
#include <iostream>

RookMovement::RookMovement(Color c) : Piece(c) {
    std::cout << "RookMovement constructor called" << std::endl;
}

RookMovement::~RookMovement() {
    std::cout << "RookMovement destructor called" << std::endl;
}

std::vector<Position> RookMovement::doGetValidMoves(Board &board, Position pos, bool checkCastling) {
    std::vector<Position> validMoves;
    //nord
    int r = pos.row - 1;
    int c = pos.col;
    while (r >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
    }
    //sud
    r = pos.row + 1;
    while (r <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
    }
    //est
    r = pos.row;
    c = pos.col - 1;
    while (c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        c --;
    }
    //vest
    c = pos.col + 1;
    while (c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        c ++;
    }
    return validMoves;
}
