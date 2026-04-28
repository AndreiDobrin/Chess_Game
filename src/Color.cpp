//
// Created by Andrei on 4/25/2026.
//

#include "Color.h"
#include <iostream>

std::ostream& operator<< (std::ostream& os, const Color& color) { // supraincarcare << pentru tipul de date Color
    switch (color) {
        case Color::WHITE:
            os << "White";
            break;
        case Color::BLACK:
            os << "Black";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}