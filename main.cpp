#include <iostream>
#include <vector>
#include <memory>
class Board;
using namespace std;

enum class Color {
    WHITE, BLACK
};

    std::ostream& operator<< (std::ostream& os, const Color& color) { // supraincarcare << pentru tipul de date Color
        switch (color) {
            case Color::WHITE:
                os << "White";
                break;
            case Color::BLACK:
                os<< "Black";
                break;
            default:
                os << "Unknown";
                break;
        }
        return os;
    }


struct Position {
    int row;
    int col;

    friend ostream& operator<< (ostream& os, const Position& pos) { // piece.position in loc de piece.position.col/piece.position.row
        os << "(" << pos.row << ", " << pos.col << ")";
        return os;
    }

};

class Piece {
    bool hasMoved; // for pawns' first move and for castling
    Color color;
    Position position;

public:
    Piece(Color c, Position startPos) : color(c), position(startPos), hasMoved(false) {} // constructor cu parametri
    Piece(Color c, Position startPos, bool hasMoved) : color(c), position(startPos), hasMoved(hasMoved) {} // constructor cu toti parametrii
    Piece() : color(Color::WHITE), position(0,0), hasMoved(false) {} // constructor fara parametri

    ~Piece() = default;

    Piece& operator= (const Piece& other) { // supraincarcare operator atribuire '='
        if (this == &other) return *this;

        this->hasMoved = other.hasMoved;
        this->color = other.color;
        this->position = other.position;

        return *this;
    }

    friend std::ostream& operator<< (std::ostream& os, const Piece& piece) {
        os << "Color: " << piece.color << endl << "hasMoved: " << piece.hasMoved; // supraincarcare <<
        return os;
    }

    friend std::istream& operator>> (std::istream& is, Piece& piece) { // supraincarcare operator '>>'
        bool hasMoved;
        string color;
        while (true) {
            cout << "Color(Black or White): ";
            is >> color;
            if (color == "White" || color == "WHITE" || color == "white" || color == "w" || color == "W") {
                piece.setColor(Color::WHITE);
                cout << endl;
                break;
            }
            if (color == "Black" || color == "BLACK" || color == "black" || color == "b" || color == "B") {
                piece.setColor(Color::BLACK);
                cout << endl;
                break;
            }
            cout << "Invalid color...\n";
        }

        while (true) {
            cout << "hasMoved(0-1): ";
            is >> hasMoved;
            if (hasMoved == 0) {
                piece.setHasMoved(false);
                cout << endl;
                break;
            }
            if (hasMoved == 1) {
                piece.setHasMoved(true);
                cout << endl;
                break;
            }
        }
        return is;
    }

    int operator[](int index) const { // supraincarcare operator '[]'
        if (index == 0) {
            return this->getPosition().row;
        }
        else if (index == 1) {
            return this->getPosition().col;
        }
        else {
            throw std::out_of_range("Index must be 0 (row) or 1 (col)");
        }
    }

    virtual vector<Position> getValidMoves(Board& board) = 0;
    virtual void printPiece() = 0;

    /*
    void setPosition (Position newPos) {
        position = newPos;
        //if (!hasMoved) {
        //    hasMoved = true;
        //}
    }
    */
    void setColor (Color newColor) {
        color = newColor;
    }

    void setHasMoved (bool newHasMoved) {
        hasMoved = newHasMoved;
    }

    Color getColor() const {
        return color;
    }
    Position getPosition() const {
        return position;
    }
    bool getHasMoved() const {
        return hasMoved;
    }
};

class Pawn : public Piece {
public:
    Pawn(Color c, Position startPos) : Piece(c, startPos) {}

    virtual void printPiece() {
        if (this->getColor() == Color::WHITE)
            cout<< "[ wP ] ";
        else {
            cout << "[ bP ] ";
        }
    }

    vector<Position> getValidMoves(Board& board);
};

class Board {

private:
    unique_ptr<Piece> grid[8][8];
public:
    Board() {
        setupBoard();
    }

    void setupBoard() {
            for (int j = 0; j < 8; j++) {
                grid[1][j] = make_unique<Pawn>(Color::BLACK, Position{1,j});
                grid[6][j] = make_unique<Pawn>(Color::WHITE, Position{6,j});
            }
    }

    void showChessboard() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] == nullptr)
                    cout << "[    ] ";
                else {
                    grid[i][j]->printPiece();
                }
            }
            cout << endl;
        }
    }

    void movePiece(Position from, Position to) {
        if (grid[from.row][from.col] == nullptr) {
            cout << "Invalid move: No piece at the starting square!" << endl;
            return;
        }

        grid[to.row][to.col] = std::move(grid[from.row][from.col]);
        grid[to.row][to.col]->setPosition(to);
        grid[to.row][to.col]->setHasMoved(true);
    }

    Piece* getPositionInfo(Position pos) {
        return grid[pos.row][pos.col].get(); // returneaza adresa din interiorul smart pointer-ului
    }

};

vector<Position> Pawn::getValidMoves(Board& board) {
    vector<Position> validMoves;
    int r = this->getPosition().row;
    int c = this->getPosition().col;
    int moveOrientation = 1;
    if (this->getColor() == Color::WHITE) {
        moveOrientation = -1;
    }
    // are liber in fata un spatiu
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 && board.getPositionInfo({r + moveOrientation, c}) == nullptr) {
        validMoves.push_back({r + moveOrientation, c});
    }
    // are liber in fata doua spatii ( + nu s-a miscat)
    if (!this->getHasMoved() && r + moveOrientation * 2 >= 0 && r + moveOrientation * 2 <= 7 &&
    board.getPositionInfo({r + moveOrientation, c}) == nullptr && // primul spatiu
    board.getPositionInfo({r + moveOrientation * 2, c}) == nullptr) { // al doilea spatiu

        validMoves.push_back({r + moveOrientation * 2, c});
    }

    // are piesa pe diagonala stanga
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 && c - 1 >= 0 &&
    board.getPositionInfo({r + moveOrientation, c - 1}) != nullptr) {
        validMoves.push_back({r + moveOrientation, c - 1});
    }

    // are piesa pe diagonala dreapta
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 && c + 1 >= 0 &&
    board.getPositionInfo({r + moveOrientation, c + 1}) != nullptr) {
        validMoves.push_back({r + moveOrientation, c + 1});
    }

    return validMoves;
}

int main() {
    Board chessboard;
    Piece* targetPiece = chessboard.getPositionInfo({1,1});
    Piece* targetPiece1 = chessboard.getPositionInfo({6,1});
    chessboard.movePiece({6,1}, {2, 0});
    chessboard.showChessboard();

    if (targetPiece != nullptr) {
        vector<Position> moves = targetPiece->getValidMoves(chessboard);
        for (Position move : moves) {
            cout << move << " ";
        }
    }

    return 0;
}