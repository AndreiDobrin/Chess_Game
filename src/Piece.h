//
// Created by Andrei on 4/25/2026.
//

#pragma once
#include <string>
#include <vector>

#include "Color.h"
#include "Position.h"
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"
#include "Globals.h"


class Board;


class Piece {
private: // datele pot fi accesate doar din clasa piece. invalid daca am accesa direct `this->color` din bishop
    bool hasMoved; // for pawns' first move and for castling
    Color color;
    SDL_Texture* img;
protected: // derivatele pot accesa/suprascrie functiile astea. valid sa o apelam din derivata, invalid din main
    virtual std::vector<Position> doGetValidMoves(Board& board, Position pos, bool checkCastling) = 0;


public: // public, oricine poate folosi functiile astea
    Piece(Color c); // constructor cu parametri
    Piece(Color c, bool hasMoved); // constructor cu toti parametrii
    Piece(); // constructor fara parametri
    Piece(const Piece &other); // copy constructor

    virtual ~Piece(); // destructor

    Piece& operator= (const Piece& other); // supraincarcare operator atribuire '='


    friend std::ostream& operator<< (std::ostream& os, const Piece& piece);

    friend std::istream& operator>> (std::istream& is, Piece& piece); // supraincarcare operator '>>'


    std::vector<Position> getValidMoves(Board& board, Position pos, bool checkCastling = true) {
        return doGetValidMoves(board, pos, checkCastling);
    }
    virtual std::string identifyPiece() = 0;
    virtual void printPiece() = 0;
    virtual void printPieceConsole() = 0; // Cerinta 3: A doua functie pur virtuala

    void setColor (Color newColor);
    void setHasMoved (bool newHasMoved);

    Color getColor() const;
    bool getHasMoved() const;
    void setImg(SDL_Texture* newImg);
    SDL_Texture* getImg();
};
