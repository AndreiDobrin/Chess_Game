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

std::string Queen::identifyPiece() {
    return "Queen";
}

std::vector<Position> Queen::doGetValidMoves(Board &board, Position pos, bool checkCastling) {
    std::vector<Position> validMoves;
    //nord-vest
    int r = pos.row - 1;
    int c = pos.col - 1;
    while (r >= 0 && c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
        c --;
    }
    //nord-est
    r = pos.row - 1;
    c = pos.col + 1;
    while (r >= 0 && c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
        c ++;
    }
    //sud-vest
    r = pos.row + 1;
    c = pos.col - 1;
    while (r <= 7 && c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
        c --;
    }
    //sud-est
    r = pos.row + 1;
    c = pos.col + 1;
    while (r <= 7 && c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
        c ++;
    }

    //nord
    r = pos.row - 1;
    c = pos.col;
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