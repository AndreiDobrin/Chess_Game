#include <iostream>
#include <vector>
#include <memory>
#include <typeinfo>
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
                os << "Black";
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
    bool operator==(const Position& other) const {
        return this->row == other.row && this->col == other.col;
    }

};

class Piece {
    bool hasMoved; // for pawns' first move and for castling
    Color color;

public:
    Piece(Color c) : color(c), hasMoved(false) {} // constructor cu parametri
    Piece(Color c, bool hasMoved) : color(c), hasMoved(hasMoved) {} // constructor cu toti parametrii
    Piece() : color(Color::WHITE), hasMoved(false) {} // constructor fara parametri
    Piece(Piece &other) = default;

    ~Piece() = default;

    Piece& operator= (const Piece& other) { // supraincarcare operator atribuire '='
        if (this == &other) return *this;

        this->hasMoved = other.hasMoved;
        this->color = other.color;

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
/*
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
*/


    virtual vector<Position> getValidMoves(Board& board, Position pos) = 0;
    virtual string identifyPiece(Board& board, Position pos) = 0;
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
    /*
    Position getPosition() const {
        return position;
    }
    */
    bool getHasMoved() const {
        return hasMoved;
    }
};

class King : public Piece {
public:
    King(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this->getColor() == Color::WHITE)
            cout << "[ wK ] ";
        else {
            cout << "[ bK ] ";
        }
    }
    vector<Position> getLegalMoves(Board& board, vector<Position> validMoves, Position pos);
    vector<Position> getValidMoves(Board& board, Position pos);
    string identifyPiece(Board& board, Position pos) {
        return "King";
    };
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this-> getColor() == Color::WHITE)
            cout << "[ wR ] ";
        else {
            cout << "[ bR ] ";
        }
    }
    vector<Position> getValidMoves(Board& board, Position pos);
    string identifyPiece(Board& board, Position pos) {
        return "Rook";
    }
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this->getColor() == Color::WHITE)
            cout<< "[ wP ] ";
        else {
            cout << "[ bP ] ";
        }
    }

    vector<Position> getValidMoves(Board& board, Position pos);
    string identifyPiece(Board& board, Position pos) {
        return "Pawn";
    }
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
                grid[1][j] = make_unique<Pawn>(Color::BLACK);
                grid[6][j] = make_unique<Pawn>(Color::WHITE);
            }
        grid[0][4] = make_unique<King>(Color::BLACK);
        grid[7][4] = make_unique<King>(Color::WHITE);
        grid[0][0] = make_unique<Rook>(Color::BLACK);
        grid[0][7] = make_unique<Rook>(Color::BLACK);
        grid[7][0] = make_unique<Rook>(Color::WHITE);
        grid[7][7] = make_unique<Rook>(Color::WHITE);

    }

    void showChessboard() {
        cout << "     ";
        for (int j = 0; j < 8; j++) {
            cout << char('A' + j) << "      ";
        }
        cout << endl;
        for (int i = 0; i < 8; i++) {
            cout << i+1 << "  ";
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

    Piece* getPositionInfo(Position pos) {
        return this->grid[pos.row][pos.col].get(); // returneaza adresa din interiorul smart pointer-ului
    }
//    bool movePiece(Position from, Position to, Color currentTurn);

    bool isKingInCheck(Color currentTurn) {
        Position kingPos;
        // cautam regele
        bool kingFound = false;
        for (int i = 0; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                Piece* selectedPiece = this->getPositionInfo({i,j});
                if (selectedPiece != nullptr && selectedPiece->identifyPiece(*this, {i,j}) == "King"
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
            cout << "CRITICAL ERROR: King is missing from the board!" << endl;
            return false;
        }

        for (int i = 0 ; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                Piece* selectedPiece = this->getPositionInfo({i,j});
                if (selectedPiece != nullptr && selectedPiece->getColor() != currentTurn) {
                    vector<Position> validMoves = this->getPositionInfo({i,j})->getValidMoves(*this,{i,j});
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

    vector<Position> getStrictlyValidMoves(Position from) {
        vector<Position> strictlyValidMoves;
        Piece* targetPiece = this->getPositionInfo(from);
        if (targetPiece == nullptr) return strictlyValidMoves;

        vector<Position> validMoves = targetPiece->getValidMoves(*this, from);
        for (Position move : validMoves) {
                if (!isCheckedAfterMove(from, move, targetPiece->getColor())) {
                    strictlyValidMoves.push_back(move);
            }
        }
        return strictlyValidMoves;
    }

    bool isCheckmate(Color currentTurn) {
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (grid[i][j] != nullptr && grid[i][j]->getColor() == currentTurn) {
                    if (!getStrictlyValidMoves({i,j}).empty())
                        return false;
                }
            }
        }
        return true;
    }





bool movePiece(Position from, Position to, Color currentTurn) {
    if (this->getPositionInfo(from) == nullptr) {
        cout << "Invalid move: No piece at "<< from << endl;
        return false;
    }
    if (this->getPositionInfo(from)->getColor() != currentTurn) {
        cout << "Invalid move: Not your piece\n";
        return false;
    }
    vector<Position> legalMoves = this->getStrictlyValidMoves(from);

    bool ok = false;
    for (Position move : legalMoves) {
        if (to == move) {
            ok = true;
            break;
        }
    }
    if (ok) {
        this->grid[to.row][to.col] = std::move(this->grid[from.row][from.col]);
        //grid[to.row][to.col]->setPosition(to);
        this->grid[to.row][to.col]->setHasMoved(true);
        return true;
    }
    cout << "Invalid move\n";
    return false;

}

bool isCheckedAfterMove(Position from, Position to, Color currentTurn) {
        bool inCheck = false;
        unique_ptr<Piece> capturedCopy = std::move(this->grid[to.row][to.col]);
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
};

vector<Position> King::getValidMoves(Board& board, Position pos) {
    vector<Position> validMoves;
    // int r = pos.row;
    // int c = pos.col;
    Color pieceColor = board.getPositionInfo(pos)->getColor();

    for (int i = pos.row - 1; i <= pos.row + 1; i++) {
        for (int j = pos.col - 1; j <= pos.col + 1; j++) {
            if (i >= 0 && i <= 7 && j >= 0 && j <= 7 &&
            (board.getPositionInfo({i,j}) == nullptr ||
            board.getPositionInfo({i,j})->getColor() != pieceColor))
                validMoves.push_back({i,j});
        }
    }
    return validMoves;
}

vector<Position> Rook::getValidMoves(Board& board, Position pos) {
    vector<Position> validMoves;
    //nord
    int r = pos.row - 1;
    int c = pos.col;
    while (r >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
    }
    //sud
    r = pos.row + 1;
    while (r <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
    }
    //est
    r = pos.row;
    c = pos.col - 1;
    while (c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        c --;
    }
    //vest
    c = pos.col + 1;
    while (c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        c ++;
    }
    return validMoves;
}


vector<Position> Pawn::getValidMoves(Board& board, Position pos) {
    vector<Position> validMoves;
    int r = pos.row;
    int c = pos.col;
    Color pieceColor = board.getPositionInfo(pos)->getColor();
    int moveOrientation = 1;
    if (this->getColor() == Color::WHITE) {
        moveOrientation = -1;
    }
    // are liber in fata un spatiu
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 &&
    board.getPositionInfo({r + moveOrientation, c}) == nullptr) {
        validMoves.push_back({r + moveOrientation, c});
    }
    // are liber in fata doua spatii ( + nu s-a miscat)
    if (!this->getHasMoved() && r + moveOrientation * 2 >= 0 && r + moveOrientation * 2 <= 7 &&
    board.getPositionInfo({r + moveOrientation, c}) == nullptr && // primul spatiu
    board.getPositionInfo({r + moveOrientation * 2, c}) == nullptr) // al doilea spatiu
    {
        validMoves.push_back({r + moveOrientation * 2, c});
    }

    // are piesa pe diagonala stanga SI ARE CULOARE DIFERITA
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 && c - 1 >= 0 &&
    board.getPositionInfo({r + moveOrientation, c - 1}) != nullptr &&
    board.getPositionInfo({r + moveOrientation, c - 1})->getColor() != pieceColor) {
        validMoves.push_back({r + moveOrientation, c - 1});
    }

    // are piesa pe diagonala dreapta SI ARE CULOARE DIFERITA
    if (r + moveOrientation >= 0 && r + moveOrientation <= 7 && c + 1 >= 0 &&
    board.getPositionInfo({r + moveOrientation, c + 1}) != nullptr &&
    board.getPositionInfo({r + moveOrientation, c + 1})->getColor() != pieceColor) {
        validMoves.push_back({r + moveOrientation, c + 1});
    }

    return validMoves;
}



class gameEngine {
    Color currentTurn;
public:
    gameEngine() {
        currentTurn = Color::WHITE;
    }

    ~gameEngine() = default;

    void playerMove(Board& board) {
        string wantedMoveFrom, wantedMoveTo;
        cin >> wantedMoveFrom;
        cin >> wantedMoveTo;
        Position from = {wantedMoveFrom[1] - '1', wantedMoveFrom[0] - 'A'};
        Position to = {wantedMoveTo[1] - '1', wantedMoveTo[0] - 'A'};
        cout << "Wanted Move: From "<< from << " to " << to << "\n";

        if (board.movePiece(from, to, currentTurn)) {
            if (currentTurn == Color::WHITE) {
                currentTurn = Color::BLACK;
            }
            else {
                currentTurn = Color::WHITE;
            }
        }
    }

    Color getCurrentTurn() {
        return currentTurn;
    }


};

int main() {
    Board chessboard;
    gameEngine Engine;
    Piece* targetPiece = chessboard.getPositionInfo({0,4});
    cout << typeid(targetPiece).name() << endl;
    Piece* targetPiece1 = chessboard.getPositionInfo({6,1});
    //chessboard.movePiece({6,1}, {2, 0});
    chessboard.showChessboard();
/*
    if (targetPiece != nullptr) {
        vector<Position> moves = targetPiece->getValidMoves(chessboard);
        for (Position move : moves) {
            cout << move << " ";
        }
    }
*/
    cout << endl;
    Engine.playerMove(chessboard);
    chessboard.showChessboard();
    while (true) {
        Engine.playerMove(chessboard);
        chessboard.showChessboard();
        if (chessboard.isKingInCheck(Engine.getCurrentTurn())) {
            cout << Engine.getCurrentTurn() << " is checked\n";
            if (chessboard.isCheckmate(Engine.getCurrentTurn())) {
                if (Engine.getCurrentTurn() == Color::WHITE)
                    cout << "BLACK WINS!!!";
                else
                    cout << "WHITE WINS!!!";
                break;
            }

        }
        else {
            if (chessboard.isCheckmate(Engine.getCurrentTurn())) {
                cout << "DRAW";
                break;
            }
        }
    }

    return 0;
}