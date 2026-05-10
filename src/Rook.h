//
// Created by Andrei on 4/26/2026.
//

#pragma once
#include "RookMovement.h"

class Rook : public RookMovement {
public:
    Rook(Color c) : Piece(c), RookMovement(c) {
        std::cout << "Rook constructor called" << std::endl;
        if (c == Color::WHITE) {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/white-rook.png"));
        }
        else {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/black-rook.png"));
        }

        if (!getImg()) {
            std::cout << "Failed to load black-rook.png and white-rook.png" << SDL_GetError() << std::endl;
        }
    }
    
    virtual ~Rook() {
        std::cout << "Rook destructor called" << std::endl;
    }

    void printPiece() override;
    void printPieceConsole() override;

    std::vector<Position> doGetValidMoves(Board& board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
};
