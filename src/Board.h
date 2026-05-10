//
// Created by Andrei on 4/26/2026.
//
#pragma once
#include "Position.h"
#include <memory>
#include "Piece.h"

// Cerinta 2: Clasa mica pentru a demonstra compunerea (has-a)
class ComponentDemo {
public:
    ComponentDemo() { std::cout << "ComponentDemo (componenta) construita." << std::endl; }
};


class Board {

private:
    ComponentDemo compDemo; // Cerinta 2: Compunere (has-a)
    std::unique_ptr<Piece> grid[8][8];

    Position lastMoveFrom;
    Position lastMoveTo;
public:
    Board();

    void setupBoard();

    void showChessboard();

    void showChessBoard(Position from, std::vector<Position> const& pos);

    void showChessBoard(Position from, Position to);

    Piece* getPositionInfo(Position pos);

    bool isKingInCheck(Color currentTurn);

    bool isPieceAttacked(Color attackedPieceColor, Position pos);

    std::vector<Position> getStrictlyValidMoves(Position from);

    bool isCheckmate(Color currentTurn);





    bool movePiece(Position from, Position to, Color currentTurn);

    bool isCheckedAfterMove(Position from, Position to, Color currentTurn);

    Position getLastMoveFrom() const;
    Position getLastMoveTo() const;
    void replacePiece(Position pos, std::unique_ptr<Piece> piece);
    void drawChessBoard(SDL_Renderer* renderer, Position from, std::vector<Position> const& pos);
    void drawChessboard(SDL_Renderer* renderer, Board& board);
};
