#pragma once
#include "Piece.h"
#include "Board.h"
#include <vector>
#include <string>

class Rook : public Piece {
    SDL_Texture* img;
public:
    Rook(Color c) : Piece(c) {
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

    void printPiece() override;

    std::vector<Position> doGetValidMoves(Board& board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
    void setPieceImages();
};


