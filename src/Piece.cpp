#include "Piece.h"
#include "Color.h"
#include <iostream>
#include <string>
#include <vector>

    Piece::Piece(const Piece &other) = default; // copy constructor
    Piece::~Piece() = default; // destructor

    Piece& Piece::operator= (const Piece& other) { // supraincarcare operator atribuire '='
        if (this == &other) return *this;

        this->hasMoved = other.hasMoved;
        this->color = other.color;

        return *this;
    }
    /*
    std::ostream& operator<< (std::ostream& os, const Piece& piece) {
        os << "Color: " << piece.color << "\nhasMoved: " << piece.hasMoved; // supraincarcare operator '<<'
        return os;
    }

    std::istream& operator>> (std::istream& is, Piece& piece) { // supraincarcare operator '>>'
        bool hasMoved;
        std::string color;
        while (true) {
            std::cout << "Color(Black or White): ";
            is >> color;
            if (color == "White" || color == "WHITE" || color == "white" || color == "w" || color == "W") {
                piece.setColor(Color::WHITE);
                std::cout << std::endl;
                break;
            }
            if (color == "Black" || color == "BLACK" || color == "black" || color == "b" || color == "B") {
                piece.setColor(Color::BLACK);
                std::cout << std::endl;
                break;
            }
            std::cout << "Invalid color...\n";
        }

        while (true) {
            std::cout << "hasMoved(0-1): ";
            is >> hasMoved;
            if (hasMoved == 0) {
                piece.setHasMoved(false);
                std::cout << std::endl;
                break;
            }
            if (hasMoved == 1) {
                piece.setHasMoved(true);
                std::cout << std::endl;
                break;
            }
        }
        return is;
    }
    */
    void Piece::setColor (Color newColor) {
        color = newColor;
    }

    void Piece::setHasMoved (bool newHasMoved) {
        hasMoved = newHasMoved;
    }

    Color Piece::getColor() const {
        return color;
    }

    bool Piece::getHasMoved() const {
        return hasMoved;
    }
    void Piece::setImg(SDL_Texture* newImg) {
        img = newImg;
    }
    SDL_Texture* Piece::getImg() {
        return img;
    }