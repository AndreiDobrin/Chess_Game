#pragma once
#include <iostream>



enum class Color {
    WHITE, BLACK


};

std::ostream& operator<< (std::ostream& os, const Color& color);


