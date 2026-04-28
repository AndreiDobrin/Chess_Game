//
// Created by Andrei on 4/26/2026.
//

#ifndef CHESS_GAME_PAWN_H
#define CHESS_GAME_PAWN_H
#include <string>
#include <vector>
#include "Piece.h"

#include "Position.h"
#include "Color.h"

#include "Globals.h"


class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c) {
        if (c == Color::WHITE) {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/white-pawn.png"));
        }
        else {
            setImg(IMG_LoadTexture(renderer, "./src/chess_pieces/black-pawn.png"));
        }
        if (!getImg()) {
            std::cout << "Failed to load black-pawn.png and white-pawn.png" << SDL_GetError() << std::endl;
        }
    }

    void printPiece() override;
    std::vector<Position> doGetValidMoves(Board& board, Position pos, bool checkCastling) override;
    //std::vector<Position> getValidMoves(Board& board, Position pos, bool checkCastling = true);
    std::string identifyPiece() override;
    bool drawPiece();
};


#endif //CHESS_GAME_PAWN_H