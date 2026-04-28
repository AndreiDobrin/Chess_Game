//
// Created by Andrei on 4/26/2026.
//
#pragma once
#include "Position.h"
#include <memory>
#include "Piece.h"


class Board {

private:
    std::unique_ptr<Piece> grid[8][8];

    Position lastMoveFrom = {-1, -1};
    Position lastMoveTo = {-1, -1};
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
    void drawChessBoard(SDL_Renderer* renderer, Position from, std::vector<Position> const& pos);
    void drawChessboard(SDL_Renderer* renderer, Board& board);
};
