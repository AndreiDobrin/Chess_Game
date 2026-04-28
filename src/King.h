#pragma once
#include "Piece.h"
#include <vector>


class King : public Piece {
    SDL_Texture* img;
public:
    King(Color c) : Piece(c) {
        if (c == Color::WHITE) {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/white-king.png"));
        }
        else {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/black-king.png"));
        }

        if (!getImg()) {
            std::cout << "Failed to load black-king.png and white-king.png" << SDL_GetError() << std::endl;
        }
    }

    void printPiece() override;
    std::vector<Position> getLegalMoves(Board& board, std::vector<Position> validMoves, Position pos);
    std::vector<Position> doGetValidMoves(Board& board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
};