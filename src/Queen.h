#pragma once
#include "BishopMovement.h"
#include "RookMovement.h"
#include <vector>
#include <string>

class Queen : public BishopMovement, public RookMovement {
public:
    Queen(Color c) : Piece(c), BishopMovement(c), RookMovement(c) {
        std::cout << "Queen constructor called" << std::endl;
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
    
    virtual ~Queen() {
        std::cout << "Queen destructor called" << std::endl;
    }

    void printPiece() override;
    void printPieceConsole() override;

    std::vector<Position> doGetValidMoves(Board &board, Position pos, bool checkCastling) override;
    std::string identifyPiece() override;
};
