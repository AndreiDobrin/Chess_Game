//
// Created by Andrei on 4/26/2026.
//

#include "Board.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include <iostream>



    Board::Board() {
        setupBoard();
    }

    void Board::setupBoard() {
        grid[0][4] = std::make_unique<King>(Color::BLACK);
        grid[7][4] = std::make_unique<King>(Color::WHITE);
        grid[0][0] = std::make_unique<Rook>(Color::BLACK);
        grid[0][7] = std::make_unique<Rook>(Color::BLACK);
        grid[7][0] = std::make_unique<Rook>(Color::WHITE);
        grid[7][7] = std::make_unique<Rook>(Color::WHITE);
        grid[0][2] = std::make_unique<Bishop>(Color::BLACK);
        grid[0][5] = std::make_unique<Bishop>(Color::BLACK);
        grid[7][2] = std::make_unique<Bishop>(Color::WHITE);
        grid[7][5] = std::make_unique<Bishop>(Color::WHITE);
        grid[0][3] = std::make_unique<Queen>(Color::BLACK);
        grid[7][3] = std::make_unique<Queen>(Color::WHITE);
        grid[0][1] = std::make_unique<Knight>(Color::BLACK);
        grid[0][6] = std::make_unique<Knight>(Color::BLACK);
        grid[7][1] = std::make_unique<Knight>(Color::WHITE);
        grid[7][6] = std::make_unique<Knight>(Color::WHITE);
            for (int j = 0; j < 8; j++) {
                grid[1][j] = std::make_unique<Pawn>(Color::BLACK);
                grid[6][j] = std::make_unique<Pawn>(Color::WHITE);
            }
    }

    void Board::showChessboard() {
        std::cout << "     ";
        for (int j = 0; j < 8; j++) {
            std::cout << char('A' + j) << "      ";
        }
        std::cout << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cout << i+1 << "  ";
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] == nullptr)
                    std::cout << "[    ]";
                else {
                    grid[i][j]->printPiece();
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    void Board::showChessBoard(Position from, std::vector<Position> const& pos) {
        std::cout << "     ";
        for (int j = 0; j < 8; j++) {
            std::cout << char('A' + j) << "      ";
        }
        std::cout << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cout << i+1 << "  ";
            for (int j = 0; j < 8; j++) {
                bool ok = false;
                //selected piece
                if (from.col == j && from.row == i) {
                    ok = true;
                    std::cout << "\033[103;30m";
                }
                //valid moves for selected piece
                else {
                    for (int k = 0; k < pos.size(); k++) {
                        if (j == pos[k].col && i == pos[k].row) {
                            ok = true;
                            std::cout << "\033[104;30m";
                        }
                    }
                }
                if (grid[i][j] == nullptr)
                    std::cout << "[    ]";
                else {
                    grid[i][j]->printPiece();
                }
                if (ok) {
                    std::cout << "\033[0m";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    void Board::showChessBoard(Position from, Position to) {
        std::cout << "     ";
        for (int j = 0; j < 8; j++) {
            std::cout << char('A' + j) << "      ";
        }
        std::cout << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cout << i+1 << "  ";
            for (int j = 0; j < 8; j++) {
                bool ok = false;
                //selected piece
                if ((from.col == j && from.row == i) || (to.col == j && to.row == i)) {
                    ok = true;
                    std::cout << "\033[103;30m";
                }
                if (grid[i][j] == nullptr)
                    std::cout << "[    ]";
                else {
                    grid[i][j]->printPiece();
                }
                if (ok) {
                    std::cout << "\033[0m";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    Piece* Board::getPositionInfo(Position pos) {
        return this->grid[pos.row][pos.col].get(); // returneaza adresa din interiorul smart pointer-ului
    }
//    bool movePiece(Position from, Position to, Color currentTurn);

    bool Board::isKingInCheck(Color currentTurn) {
        Position kingPos;
        // cautam regele
        bool kingFound = false;
        for (int i = 0; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                Piece* selectedPiece = this->getPositionInfo({i,j});
                if (selectedPiece != nullptr && selectedPiece->identifyPiece() == "King"
                && selectedPiece->getColor() == currentTurn) {
                    kingPos = {i,j};
                    kingFound = true;
                    break;
                }
            }
            if (kingFound) {
                break;
            }
        }
        if (!kingFound) {
            std::cout << "CRITICAL ERROR: King is missing from the board!" << std::endl;
            return false;
        }

        for (int i = 0 ; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                Piece* selectedPiece = this->getPositionInfo({i,j});
                if (selectedPiece != nullptr && selectedPiece->getColor() != currentTurn) {
                    std::vector<Position> validMoves = this->getPositionInfo({i,j})->getValidMoves(*this,{i,j}, false);
                    for (Position move : validMoves) {
                        if (move == kingPos) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool Board::isPieceAttacked(Color attackedPieceColor, Position pos) {
            for (int i = 0 ; i <= 7; i++) {
                for (int j = 0; j <= 7; j++) {
                    Piece* selectedPiece = this->getPositionInfo({i,j});
                    if (selectedPiece != nullptr && selectedPiece->getColor() != attackedPieceColor) {
                        std::vector<Position> validMoves = this->getPositionInfo({i,j})->getValidMoves(*this,{i,j}, false);
                        for (Position move : validMoves) {
                            if (move == pos) {
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }

    std::vector<Position> Board::getStrictlyValidMoves(Position from) {
        std::vector<Position> strictlyValidMoves;
        Piece* targetPiece = this->getPositionInfo(from);
        if (targetPiece == nullptr) return strictlyValidMoves;

        std::vector<Position> validMoves = targetPiece->getValidMoves(*this, from);
        for (Position move : validMoves) {
                if (!isCheckedAfterMove(from, move, targetPiece->getColor())) {
                    strictlyValidMoves.push_back(move);
            }
        }
        return strictlyValidMoves;
    }

    bool Board::isCheckmate(Color currentTurn) {
        for (int i = 0; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                if (grid[i][j] != nullptr && grid[i][j]->getColor() == currentTurn) {
                    if (!getStrictlyValidMoves({i,j}).empty())
                        return false;
                }
            }
        }
        return true;
    }





bool Board::movePiece(Position from, Position to, Color currentTurn) {
    if (this->getPositionInfo(from) == nullptr) {
        std::cout << "Invalid move: No piece at "<< from << std::endl;
        return false;
    }
    if (this->getPositionInfo(from)->getColor() != currentTurn) {
        std::cout << "Invalid move: Not your piece\n";
        return false;
    }
    std::vector<Position> legalMoves = this->getStrictlyValidMoves(from);

    bool ok = false;
    for (Position move : legalMoves) {
        if (to == move) {
            ok = true;
            break;
        }
    }
    if (ok) {
        // check for castling
        if (getPositionInfo(from)->identifyPiece() == "King") {
            if (abs(from.col - to.col) == 2) {
                if (to.row == 7 && to.col == 2) {
                    this->grid[7][3] = std::move(this->grid[7][0]);
                }
                if (to.row == 7 && to.col == 6) {
                    this->grid[7][5] = std::move(this->grid[7][7]);
                }
                if (to.row == 0 && to.col == 2) {
                    this->grid[0][3] = std::move(this->grid[0][0]);
                }
                if (to.row == 0 && to.col == 6) {
                    this->grid[0][5] = std::move(this->grid[0][7]);
                }
            }
        }

        //check for en passant
        if (getPositionInfo(from)->identifyPiece() == "Pawn") {
            if (abs(from.col - to.col) == 1) {
                if (getPositionInfo(to) == nullptr) {
                    int moveOrientation = (currentTurn == Color::WHITE ? 1 : -1);
                    grid[to.row + moveOrientation][to.col] = nullptr;
                }
            }
        }
        this->grid[to.row][to.col] = std::move(this->grid[from.row][from.col]);
        //grid[to.row][to.col]->setPosition(to);
        this->grid[to.row][to.col]->setHasMoved(true);

        this->lastMoveFrom = from;
        this->lastMoveTo = to;

        return true;
    }
    std::cout << "Invalid move\n";
    return false;

}

bool Board::isCheckedAfterMove(Position from, Position to, Color currentTurn) {
        bool inCheck = false;
        std::unique_ptr<Piece> capturedCopy = std::move(this->grid[to.row][to.col]);
        grid[to.row][to.col] = std::move(this->grid[from.row][from.col]);
        if (this->isKingInCheck(currentTurn)) {
            // validMoves.erase(validMoves.begin() + i);
            //cout << "Invalid move: King is checked after this move\n";
            inCheck = true;
        }
        grid[from.row][from.col] = std::move(this->grid[to.row][to.col]);
        this->grid[to.row][to.col] = std::move(capturedCopy);
        return inCheck;
    }

    Position Board::getLastMoveFrom() const { return lastMoveFrom; }
    Position Board::getLastMoveTo() const { return lastMoveTo; }


void Board::drawChessBoard(SDL_Renderer* renderer, Position from, std::vector<Position> const& pos) {
        SDL_FRect rect = {xOffset, yOffset, 50.0f, 50.0f};
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                rect.x = xOffset + 50.0f * j;
                rect.y = yOffset + 50.0f * i;
                    if ((i + j) % 2 == 0) {
                        SDL_SetRenderDrawColor(renderer, 90, 56, 47, 255);
                    }
                    else {
                        SDL_SetRenderDrawColor(renderer, 149, 83, 59, 255);
                    }

                    if (from.col == j && from.row == i) {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                    }
                    //valid moves for selected piece
                    else {
                        for (int k = 0; k < pos.size(); k++) {
                            if (j == pos[k].col && i == pos[k].row) {
                                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                            }
                        }
                    }
                SDL_RenderFillRect(renderer, &rect);
                if (this->getPositionInfo({i,j}) != nullptr) {
                    SDL_RenderTexture(renderer, this->getPositionInfo({i,j})->getImg(), nullptr, &rect);
                }
            }
        }
    }



void Board::drawChessboard(SDL_Renderer* renderer, Board& board) {
        SDL_FRect rect = {xOffset, yOffset, 50.0f, 50.0f};
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                rect.x = xOffset + 50.0f * j;
                rect.y = yOffset + 50.0f * i;

                if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 90, 56, 47, 255);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 149, 83, 59, 255);
                }
                SDL_RenderFillRect(renderer, &rect);
                if (board.getPositionInfo({i,j}) != nullptr) {
                    SDL_RenderTexture(renderer, board.getPositionInfo({i,j})->getImg(), nullptr, &rect);
                }
            }
        }
    }