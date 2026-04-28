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
    std::vector<Position> Rook::doGetValidMoves(Board& board, Position pos, bool checkCastling) {
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
    std::string Rook::identifyPiece() {
        return "Rook";
    }
