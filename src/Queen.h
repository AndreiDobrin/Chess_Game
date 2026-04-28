#pragma once
#include "Piece.h"
#include <vector>
#include <string>



class Queen : public Piece {
    SDL_Texture* img;
public:
    Queen(Color c) : Piece(c) {
        if (c == Color::WHITE) {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/white-queen.png"));
        }
        else {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/black-queen.png"));
        }

        if (!getImg()) {
            std::cout << "Failed to load black-queen.png and white-queen.png" << SDL_GetError() << std::endl;
        }
    }

    void printPiece() override;

    std::vector<Position> doGetValidMoves(Board &board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
};

