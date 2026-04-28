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

    std::vector<Position> Bishop::doGetValidMoves(Board &board, Position pos, bool checkCastling){
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


        return validMoves;

    }
    std::string Bishop::identifyPiece() {
        return "Bishop";
    }