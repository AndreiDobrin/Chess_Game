//
// Created by Andrei on 4/26/2026.
//

#pragma once
#include "Board.h"

class Bishop : public Piece {
    SDL_Texture* img;
public:
    Bishop(Color c) : Piece(c) {
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

    void printPiece() override;

    std::vector<Position> doGetValidMoves(Board &board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
    void setPieceImages();
};

