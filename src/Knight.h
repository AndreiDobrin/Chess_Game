#pragma once
#include <string>
#include <vector>
#include "Position.h"
#include "Board.h"
#include "Piece.h"


class Knight : public Piece {
    SDL_Texture* img;
public:
    Knight(Color c) : Piece(c) {
        if (c == Color::WHITE) {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/white-knight.png"));
        }
        else {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/black-knight.png"));
        }

        if (!getImg()) {
            std::cout << "Failed to load black-knight.png and white-knight.png" << SDL_GetError() << std::endl;
        }
    }

    void printPiece() override;

    std::vector<Position> doGetValidMoves(Board& board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
};


