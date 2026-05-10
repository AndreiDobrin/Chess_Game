//
// Created by Andrei on 4/26/2026.
//

#pragma once
#include "BishopMovement.h"

class Bishop : public BishopMovement {
public:
    Bishop(Color c) : Piece(c), BishopMovement(c) {
        std::cout << "Bishop constructor called" << std::endl;
        if (c == Color::WHITE) {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/white-bishop.png"));
        }
        else {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/black-bishop.png"));
        }

        if (!getImg()) {
            std::cout << "Failed to load black-bishop.png and white-bishop.png" << SDL_GetError() << std::endl;
        }
    }
    
    virtual ~Bishop() {
        std::cout << "Bishop destructor called" << std::endl;
    }

    void printPiece() override;
    void printPieceConsole() override;

    std::vector<Position> doGetValidMoves(Board &board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
    void setPieceImages();
};
