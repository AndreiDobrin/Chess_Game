#include <iostream>
#include <vector>
#include <memory>
#include <typeinfo>
#include <chrono>
#include <cstring>
#include <string>
#include <tuple>
#include "sqlite3.h"
#include <stdio.h>
#include <csignal>
#ifdef _WIN32
#include <windows.h>
#endif

class Board;
using namespace std;

enum class Color {
    WHITE, BLACK
};



sqlite3* db;

void openDB(const char* filename) {
    int opened = sqlite3_open(filename, &db);
    if (opened) {
        printf("Database could not be opened %s\n", sqlite3_errmsg(db));
    } else {
        //printf("Opened database successfully\n");
    }
}

void initialiseDB() {
    char* errMsg;
    const char* sql =
                "CREATE TABLE IF NOT EXISTS MATCHES("
                "MATCH_ID INTEGER PRIMARY KEY,"
                "DATE TEXT DEFAULT CURRENT_TIMESTAMP NOT NULL,"
                "WHITE TEXT DEFAULT 'GUEST' NOT NULL,"
                "BLACK TEXT DEFAULT 'GUEST' NOT NULL,"
                "RESULT TEXT DEFAULT 'ABANDONED' NOT NULL);"

                "CREATE TABLE IF NOT EXISTS MATCH_MOVES("
                "MOVE_ID INTEGER PRIMARY KEY,"
                "MATCH_ID INTEGER,"
                "MOVE TEXT,"
                "MOVE_NUMBER INTEGER,"
                "TIME_TO_MOVE REAL,"
                "PLAYER TEXT,"
                "FOREIGN KEY (MATCH_ID) REFERENCES MATCHES(MATCH_ID))";
    int rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
    if(rc != SQLITE_OK){
        printf("Error in executing SQL create: %s \n", errMsg);
        //free the error message
        sqlite3_free(errMsg);
    } else {
        printf("DB initialised successfully\n");

    }
}

void insertData() {
    char* errMsg;
    const char* sql = "INSERT INTO MATCHES (WHITE, BLACK, RESULT) "
                "VALUES('TEST1', 'TEST2', '1-0')";
    int rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("error occured %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("operation insert successful \n");
    }
}



void getData() {
    char* errMsg;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT WHITE, BLACK, RESULT FROM MATCHES;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        printf("error occurred %s\n", sqlite3_errmsg(db));
    } else {
        printf("operation get successful \n ");
    }
    printf("--- MATCH HISTORY ---\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // 3. Read the columns for the current row
        // Columns are 0-indexed based on your SELECT statement

        // Column 0: WHITE
        const unsigned char* white = sqlite3_column_text(stmt, 0);

        // Column 1: BLACK
        const unsigned char* black = sqlite3_column_text(stmt, 1);

        // Column 2: RESULT
        const unsigned char* result = sqlite3_column_text(stmt, 2);

        // Print them out (casting them to standard C strings)
        printf("%s vs %s | Result: %s\n",
               (const char*)white,
               (const char*)black,
               (const char*)result);
    }

    sqlite3_finalize(stmt);
}


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
    Piece(Piece &other) = default; // copy constructor

    ~Piece() = default; // destructor

    Piece& operator= (const Piece& other) { // supraincarcare operator atribuire '='
        if (this == &other) return *this;

        this->hasMoved = other.hasMoved;
        this->color = other.color;

        return *this;
    }

    friend std::ostream& operator<< (std::ostream& os, const Piece& piece) {
        os << "Color: " << piece.color << endl << "hasMoved: " << piece.hasMoved; // supraincarcare operator '<<'
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
    //
    // int operator[](int index) const { // supraincarcare operator '[]'
    //     if (index == 0) {
    //         return this->getPosition().row;
    //     }
    //     else if (index == 1) {
    //         return this->getPosition().col;
    //     }
    //     else {
    //         throw std::out_of_range("Index must be 0 (row) or 1 (col)");
    //     }
    // }



    virtual vector<Position> getValidMoves(Board& board, Position pos, bool checkCastling = true) = 0;
    virtual string identifyPiece() = 0;
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
            cout << "[ wK ]";
        else {
            cout << "[ bK ]";
        }
    }
    vector<Position> getLegalMoves(Board& board, vector<Position> validMoves, Position pos);
    vector<Position> getValidMoves(Board& board, Position pos, bool checkCastling = true) override;
    string identifyPiece() {
        return "King";
    };
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this-> getColor() == Color::WHITE)
            cout << "[ wR ]";
        else {
            cout << "[ bR ]";
        }
    }
    vector<Position> getValidMoves(Board& board, Position pos, bool checkCastling = true);
    string identifyPiece() {
        return "Rook";
    }
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this->getColor() == Color::WHITE)
            cout<< "[ wP ]";
        else {
            cout << "[ bP ]";
        }
    }

    vector<Position> getValidMoves(Board& board, Position pos, bool checkCastling = true);
    string identifyPiece() {
        return "Pawn";
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this->getColor() == Color::WHITE)
            cout << "[ wB ]";
        else
            cout << "[ bB ]";
    }

    vector<Position> getValidMoves(Board &board, Position pos, bool checkCastling = true);
    string identifyPiece() {
        return "Bishop";
    }
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this->getColor() == Color::WHITE)
            cout << "[ wQ ]";
        else
            cout << "[ bQ ]";
    }

    vector<Position> getValidMoves(Board &board, Position pos, bool checkCastling = true);
    string identifyPiece() {
        return "Queen";
    }
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c) {}

    virtual void printPiece() {
        if (this->getColor() == Color::WHITE)
            cout << "[ wN ]";
        else
            cout << "[ bN ]";
    }

    vector<Position> getValidMoves(Board &board, Position pos, bool checkCastling = true);
    string identifyPiece() {
        return "Knight";
    }
};

class Board {

private:
    unique_ptr<Piece> grid[8][8];

    Position lastMoveFrom = {-1, -1};
    Position lastMoveTo = {-1, -1};
public:
    Board() {
        setupBoard();
    }

    void setupBoard() {
        grid[0][4] = make_unique<King>(Color::BLACK);
        grid[7][4] = make_unique<King>(Color::WHITE);
        grid[0][0] = make_unique<Rook>(Color::BLACK);
        grid[0][7] = make_unique<Rook>(Color::BLACK);
        grid[7][0] = make_unique<Rook>(Color::WHITE);
        grid[7][7] = make_unique<Rook>(Color::WHITE);
        grid[0][2] = make_unique<Bishop>(Color::BLACK);
        grid[0][5] = make_unique<Bishop>(Color::BLACK);
        grid[7][2] = make_unique<Bishop>(Color::WHITE);
        grid[7][5] = make_unique<Bishop>(Color::WHITE);
        grid[0][3] = make_unique<Queen>(Color::BLACK);
        grid[7][3] = make_unique<Queen>(Color::WHITE);
        grid[0][1] = make_unique<Knight>(Color::BLACK);
        grid[0][6] = make_unique<Knight>(Color::BLACK);
        grid[7][1] = make_unique<Knight>(Color::WHITE);
        grid[7][6] = make_unique<Knight>(Color::WHITE);
            for (int j = 0; j < 8; j++) {
                grid[1][j] = make_unique<Pawn>(Color::BLACK);
                grid[6][j] = make_unique<Pawn>(Color::WHITE);
            }


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
                    cout << "[    ]";
                else {
                    grid[i][j]->printPiece();
                }
                cout << " ";
            }
            cout << endl;
        }
    }

    void showChessBoard(Position from, vector<Position> pos) {
        cout << "     ";
        for (int j = 0; j < 8; j++) {
            cout << char('A' + j) << "      ";
        }
        cout << endl;
        for (int i = 0; i < 8; i++) {
            cout << i+1 << "  ";
            for (int j = 0; j < 8; j++) {
                bool ok = false;
                //selected piece
                if (from.col == j && from.row == i) {
                    ok = true;
                    cout << "\033[103;30m";
                }
                //valid moves for selected piece
                else {
                    for (int k = 0; k < pos.size(); k++) {
                        if (j == pos[k].col && i == pos[k].row) {
                            ok = true;
                            cout << "\033[104;30m";
                        }
                    }
                }
                if (grid[i][j] == nullptr)
                    cout << "[    ]";
                else {
                    grid[i][j]->printPiece();
                }
                if (ok) {
                    cout << "\033[0m";
                }
                cout << " ";
            }
            cout << endl;
        }
    }

    void showChessBoard(Position from, Position to) {
        cout << "     ";
        for (int j = 0; j < 8; j++) {
            cout << char('A' + j) << "      ";
        }
        cout << endl;
        for (int i = 0; i < 8; i++) {
            cout << i+1 << "  ";
            for (int j = 0; j < 8; j++) {
                bool ok = false;
                //selected piece
                if ((from.col == j && from.row == i) || (to.col == j && to.row == i)) {
                    ok = true;
                    cout << "\033[103;30m";
                }
                if (grid[i][j] == nullptr)
                    cout << "[    ]";
                else {
                    grid[i][j]->printPiece();
                }
                if (ok) {
                    cout << "\033[0m";
                }
                cout << " ";
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
            cout << "CRITICAL ERROR: King is missing from the board!" << endl;
            return false;
        }

        for (int i = 0 ; i <= 7; i++) {
            for (int j = 0; j <= 7; j++) {
                Piece* selectedPiece = this->getPositionInfo({i,j});
                if (selectedPiece != nullptr && selectedPiece->getColor() != currentTurn) {
                    vector<Position> validMoves = this->getPositionInfo({i,j})->getValidMoves(*this,{i,j}, false);
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

    bool isPieceAttacked(Color attackedPieceColor, Position pos) {
            for (int i = 0 ; i <= 7; i++) {
                for (int j = 0; j <= 7; j++) {
                    Piece* selectedPiece = this->getPositionInfo({i,j});
                    if (selectedPiece != nullptr && selectedPiece->getColor() != attackedPieceColor) {
                        vector<Position> validMoves = this->getPositionInfo({i,j})->getValidMoves(*this,{i,j}, false);
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

    Position getLastMoveFrom() const { return lastMoveFrom; }
    Position getLastMoveTo() const { return lastMoveTo; }
};

vector<Position> King::getValidMoves(Board& board, Position pos, bool checkCastling) {
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

    // castling
    if (checkCastling && this->getHasMoved() == false && !board.isKingInCheck(pieceColor)) {
        if (pieceColor == Color::WHITE) {
            if (board.getPositionInfo({7,7}) != nullptr) {
                if (board.getPositionInfo({7, 7})->getHasMoved() == false &&
                   board.getPositionInfo({7, 6}) == nullptr &&
                   board.getPositionInfo({7, 5}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {7,5})) {
                        validMoves.push_back({7, 6});
                    }
                   }
            }
            if (board.getPositionInfo({7,0}) != nullptr) {
                if (board.getPositionInfo({7, 0})->getHasMoved() == false &&
                   board.getPositionInfo({7, 1}) == nullptr &&
                   board.getPositionInfo({7, 2}) == nullptr &&
                   board.getPositionInfo({7, 3}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {7,3}) &&
                        !board.isPieceAttacked(Color::WHITE, {7,2})) {

                        validMoves.push_back({7, 2});
                    }
                   }
            }
        }

        if (pieceColor == Color::BLACK) {
            if (board.getPositionInfo({0,0}) != nullptr) {
                if (board.getPositionInfo({0,0})->getHasMoved() == false &&
                   board.getPositionInfo({0,1}) == nullptr &&
                   board.getPositionInfo({0,2}) == nullptr &&
                   board.getPositionInfo({0,3}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {0,3}) &&
                        !board.isPieceAttacked(Color::WHITE, {0,2})) {

                        validMoves.push_back({0,2});
                    }
                   }
            }
            if (board.getPositionInfo({0,7}) != nullptr) {
                if (board.getPositionInfo({0,7})->getHasMoved() == false &&
                   board.getPositionInfo({0,6}) == nullptr &&
                   board.getPositionInfo({0,5}) == nullptr) {

                    if (!board.isPieceAttacked(Color::WHITE, {0,5})) {
                        validMoves.push_back({0,6});
                    }
                   }
            }
        }
    }

    return validMoves;
}

vector<Position> Rook::getValidMoves(Board& board, Position pos, bool checkCastling) {
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


vector<Position> Pawn::getValidMoves(Board& board, Position pos, bool checkCastling) {
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

    // en passant
    if (board.getLastMoveFrom().row != -1 && board.getLastMoveFrom().col != -1) {
        Position lastFrom = board.getLastMoveFrom();
        Position lastTo = board.getLastMoveTo();
        Piece* lastPiece = board.getPositionInfo(lastTo);

        if (lastPiece != nullptr &&
            lastPiece->getColor() != this->getColor() &&
            lastPiece->identifyPiece() == "Pawn") {

            if (abs(lastFrom.row - lastTo.row) == 2) {

                if (pos.row == lastTo.row && abs(pos.col - lastTo.col) == 1) {

                    validMoves.push_back({pos.row + moveOrientation, lastTo.col});
                }
            }
            }
    }

    return validMoves;
}

vector<Position> Bishop::getValidMoves(Board &board, Position pos, bool checkCastling) {
    vector<Position> validMoves;
    //nord-vest
    int r = pos.row - 1;
    int c = pos.col - 1;
    while (r >= 0 && c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
        c --;
    }
    //nord-est
    r = pos.row - 1;
    c = pos.col + 1;
    while (r >= 0 && c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
        c ++;
    }
    //sud-vest
    r = pos.row + 1;
    c = pos.col - 1;
    while (r <= 7 && c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
        c --;
    }
    //sud-est
    r = pos.row + 1;
    c = pos.col + 1;
    while (r <= 7 && c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
        c ++;
    }


    return validMoves;
}

vector<Position> Queen::getValidMoves(Board &board, Position pos, bool checkCastling) {
    vector<Position> validMoves;
    //nord-vest
    int r = pos.row - 1;
    int c = pos.col - 1;
    while (r >= 0 && c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
        c --;
    }
    //nord-est
    r = pos.row - 1;
    c = pos.col + 1;
    while (r >= 0 && c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r --;
        c ++;
    }
    //sud-vest
    r = pos.row + 1;
    c = pos.col - 1;
    while (r <= 7 && c >= 0) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
        c --;
    }
    //sud-est
    r = pos.row + 1;
    c = pos.col + 1;
    while (r <= 7 && c <= 7) {
        if (board.getPositionInfo({r,c}) != nullptr) {
            if (board.getPositionInfo({r,c})->getColor() == board.getPositionInfo(pos)->getColor())
                break;
            validMoves.push_back({r,c});
            break;
        }
        validMoves.push_back({r,c});
        r ++;
        c ++;
    }

    //nord
    r = pos.row - 1;
    c = pos.col;
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



vector<Position> Knight::getValidMoves(Board &board, Position pos, bool checkCastling) {
    vector<Position> validMoves;

    int r = pos.row;
    int c = pos.col;

    //dreapta sus
    // _ _ _
    // |
    int r1 = r - 1;
    int c1 = c + 2;
    if (r1 >= 0 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //stanga sus
    // _ _ _
    //     |
    r1 = r - 1;
    c1 = c - 2;
    if (r1 >= 0 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //dreapta jos
    // |
    // _ _ _
    r1 = r + 1;
    c1 = c + 2;
    if (r1 <= 7 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //stanga jos
    //     |
    // _ _ _
    r1 = r + 1;
    c1 = c - 2;
    if (r1 <= 7 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    // _ _
    // |
    // |
    r1 = r - 2;
    c1 = c + 1;
    if (r1 >= 0 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    // _ _
    //   |
    //   |
    r1 = r - 2;
    c1 = c - 1;
    if (r1 >= 0 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    // |
    // |
    // _ _
    r1 = r + 2;
    c1 = c + 1;
    if (r1 <= 7 && c1 <= 7) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    //   |
    //   |
    // _ _
    r1 = r + 2;
    c1 = c - 1;
    if (r1 <= 7 && c1 >= 0) {
        if (board.getPositionInfo({r1,c1}) != nullptr) {
            if (board.getPositionInfo({r1,c1})->getColor() != board.getPositionInfo(pos)->getColor())
                validMoves.push_back({r1,c1});
        }
        else
            validMoves.push_back({r1,c1});
    }
    return validMoves;
}


class gameEngine {
    Color currentTurn;
    // piesa mutata; captureaza piesa?; from; to.
    vector<tuple<char, bool, Position, Position>> moveHistory;
    float* moveDurations;
    int movesCapacity;
    char whitePlayer[21];
    char blackPlayer[21];

    void expandCapacity() {
        int newCapacity = movesCapacity * 2;
        float* newMoveDurations = new float[newCapacity];

        for (int i = 0; i < movesCapacity; i++) {
            newMoveDurations[i] = moveDurations[i];
        }

        delete[] moveDurations;
        movesCapacity = newCapacity;
        moveDurations = newMoveDurations;
        cout << "Array size expanded to " << movesCapacity << endl;
    }
public:
    gameEngine() {
        currentTurn = Color::WHITE;
        movesCapacity = 10;
        moveDurations = new float[movesCapacity];
    }

    ~gameEngine() {
        delete[] moveDurations;
    };
    gameEngine(gameEngine &other) {
        this->currentTurn = other.currentTurn;
        this->moveHistory = other.moveHistory;
        this->movesCapacity = other.movesCapacity;
        this->moveDurations = new float[this->movesCapacity];

        for (int i = 0; i < this->movesCapacity; i++) {
            this->moveDurations[i] = other.moveDurations[i];
        }
    }

    bool playerMove(Board& board, std::chrono::steady_clock::time_point startTime) {
        string wantedMoveFrom, wantedMoveTo;
        cin >> wantedMoveFrom;
        Position from = {wantedMoveFrom[1] - '1', toupper(wantedMoveFrom[0]) - 'A'};
        if (from.row > 7 || from.row < 0 || from.col > 7 || from.col < 0) {
            cout << "Invalid move: Out of bounds\n";
            board.showChessboard();
            return false;
        }
        if (board.getPositionInfo(from) == nullptr) {
            cout << "No piece selected\n";
            board.showChessboard();
            return false;
        }
        vector<Position> wantedValidPositions = board.getPositionInfo(from)->getValidMoves(board, from);
        board.showChessBoard(from, wantedValidPositions);

        cin >> wantedMoveTo;
        Position to = {wantedMoveTo[1] - '1', toupper(wantedMoveTo[0]) - 'A'};
        cout << "Wanted Move: From "<< from << " to " << to << "\n";
        if (to.row > 7 || to.row < 0 || to.col > 7 || to.col < 0) {
            cout << "Invalid move: Out of bounds\n";
            board.showChessboard();
            return false;
        }
        if (board.getPositionInfo(from) == nullptr) {
            cout << "No piece selected\n";
            board.showChessboard();
            return false;
        }

        bool toHasEnemyPiece = board.getPositionInfo(to) != nullptr && board.getPositionInfo(to)->getColor() != currentTurn;

        char fromPiece = board.getPositionInfo(from)->identifyPiece()[0];

        if (board.movePiece(from, to, currentTurn)) {
            auto endTime = chrono::steady_clock::now();
            moveHistory.push_back(make_tuple(fromPiece, toHasEnemyPiece, from,to));
            if (moveHistory.size() >= movesCapacity) {
                expandCapacity();
            }
            moveDurations[moveHistory.size() - 1] = chrono::duration<float>(endTime - startTime).count();
            if (currentTurn == Color::WHITE) {
                currentTurn = Color::BLACK;
            }
            else {

                currentTurn = Color::WHITE;
            }
            board.showChessBoard(from, to);
            return true;
        }
        board.showChessboard();
        return false;
    }

    Color getCurrentTurn() {
        return currentTurn;
    }
    vector<tuple<char, bool, Position, Position>> getMoveHistory() {
        return moveHistory;
    }
    float* getMoveDuration() {
        return moveDurations;
    }
    int getMoveHistorySize() {
        return moveHistory.size();
    }
    void setPlayerName() {
        cout << "White player's name: ";
        cin.get(whitePlayer, 21);
        if (!cin) {
            cin.clear();
            strcpy(whitePlayer, "GUEST");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Black player's name: ";
        cin.get(blackPlayer, 20);
        if (!cin) {
            cin.clear();
            strcpy(blackPlayer, "GUEST");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "------- " << whitePlayer << " VS. " << blackPlayer << " --------\n";
    }
    const char* getPlayerName(Color color) {
        if (color == Color::WHITE)
            return whitePlayer;

        return blackPlayer;
    }

    string getMove(int i) {
        if (i >= moveHistory.size())
            return "Index too big";

        string move = "";
        if (get<0>(moveHistory[i]) != 'P') {
            char piece = get<0>(moveHistory[i]);
            move += piece;
        }

        if (get<1>(moveHistory[i]))
            move += "x";
        move += tolower(get<3>(moveHistory[i]).col + 'A');
        move += to_string(get<3>(moveHistory[i]).row + 1);
        return move;
    }

    vector<Position> getLastMove() {
        return {get<2>(moveHistory[moveHistory.size() - 1]), get<3>(moveHistory[moveHistory.size() - 1])};
    }

};




std::ostream& operator<< (std::ostream& os, const vector<tuple<char, bool, Position, Position>>& moveHistory) { // supraincarcare << pentru moveHistory
    for (int i = 0; i < moveHistory.size(); i++) {
        os << i + 1 << ".: ";// << get<0>(moveHistory[i]);
        if (get<0>(moveHistory[i]) != 'P')
            os << get<0>(moveHistory[i]);
        if (get<1>(moveHistory[i]))
            os << 'x';
        os << char(tolower(char(get<3>(moveHistory[i]).col + 'A'))) << get<3>(moveHistory[i]).row + 1 << "\n";
    }
    return os;
}

std::ostream& operator<< (std::ostream& os, const tuple<char, bool, Position, Position>& moveHistory) { // supraincarcare << pentru moveHistory
        if (get<0>(moveHistory) != 'P')
            os << get<0>(moveHistory);
        if (get<1>(moveHistory))
            os << 'x';
        os << char(tolower(char(get<3>(moveHistory).col + 'A'))) << get<3>(moveHistory).row + 1;
    return os;
}

long long insertMatch(const char* whitePlayer, const char* blackPlayer) {
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "INSERT INTO MATCHES(WHITE, BLACK) VALUES(?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Error inserting match in database\n";
        return -1;
    }
    if (sqlite3_bind_text(stmt, 1, whitePlayer, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, blackPlayer, -1, SQLITE_TRANSIENT) != SQLITE_OK
        ) {

        cout << "Failed to bind variables: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return -1;
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return -1;
    }

    cout << "Match inserted in database successfully\n";
    sqlite3_finalize(stmt);
    return sqlite3_last_insert_rowid(db);
}

// void insertMove(const char* move, float duration, int turnNr, Color currentTurn, long long matchId) {
//     sqlite3_stmt* stmt = nullptr;
//     const char* sql = "INSERT INTO MATCH_MOVES(MATCH_ID, MOVE, MOVE_NUMBER, TIME_TO_MOVE, PLAYER) VALUES(?, ?, ?, ?, ?);";
//     int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
//     if (rc != SQLITE_OK) {
//         cout << "Error inserting move in database\n";
//         return;
//     }
//     char color[6];
//     if (currentTurn == Color::WHITE)
//         strcpy(color, "WHITE");
//     else
//         strcpy(color, "BLACK");
//
//     if (sqlite3_bind_int64(stmt, 1, matchId) != SQLITE_OK ||
//         sqlite3_bind_text(stmt, 1, move, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
//         sqlite3_bind_int(stmt, 1, turnNr) != SQLITE_OK ||
//         sqlite3_bind_double(stmt, 1, duration) != SQLITE_OK ||
//         sqlite3_bind_text(stmt, 1, color, -1, SQLITE_TRANSIENT) != SQLITE_OK
//         ) {
//         cout << "Failed to bind variables: " << sqlite3_errmsg(db) << endl;
//         sqlite3_finalize(stmt);
//         return;
//     }
//     if (sqlite3_step(stmt) != SQLITE_DONE) {
//         cout << "Execution failed: " << sqlite3_errmsg(db) << endl;
//         sqlite3_finalize(stmt);
//         return;
//     }
//     cout << "Move inserted in db successfully\n";
//     sqlite3_finalize(stmt);
//
// }

void insertWin(long long matchId, int result) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE TABLE MATCHES"
                      "SET RESULT = ?"
                      "WHERE MATCH_ID = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Error inserting win in database\n";
        return;
    }
    string resultText;
    switch (result) {
        case -1:
            resultText = "1/2-1/2.";
        case 0:
            resultText = "1-0.";
        case 1:
            resultText = "0-1.";
    }

    if (sqlite3_bind_text(stmt, 1, resultText.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, matchId) != SQLITE_OK
        ) {
        cout << "Failed to bind variables(line 1177): " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
        }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }
    cout << "Match result inserted in db successfully\n";
    sqlite3_finalize(stmt);
}

vector<tuple<long long, string, string, string, string>> getMatchIds(int offset, int limit) {
    sqlite3_stmt* stmt = nullptr;
    vector<tuple<long long, string, string, string, string>> matchIds;
    const char* sql = "SELECT * FROM MATCHES "
                 "ORDER BY MATCH_ID DESC "
                 "LIMIT ? OFFSET ?;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Statement error on searching matches: \n";
        return matchIds;
    }
    if (sqlite3_bind_int(stmt, 1, limit) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, offset) != SQLITE_OK
        ) {
        cout << "Failed to bind variables(line 1203): " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return matchIds;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        tuple<long long, string, string, string, string> selectResult;

        get<0>(selectResult) = sqlite3_column_int64(stmt,0);

        const unsigned char* text = sqlite3_column_text(stmt, 1);
        get<1>(selectResult) = text ? (const char*)text : "Unknown";

        text = sqlite3_column_text(stmt, 2);
        get<2>(selectResult) = text ? (const char*)text : "Unknown";

        text = sqlite3_column_text(stmt, 3);
        get<3>(selectResult) = text ? (const char*)text : "Unknown";

        text = sqlite3_column_text(stmt, 4);
        get<4>(selectResult) = text ? (const char*)text : "Unknown";
        matchIds.push_back(selectResult);
    }
    sqlite3_finalize(stmt);
    return matchIds;
}

int getTotalMatches() {
    sqlite3_stmt* stmt = nullptr;
    int totalMatches;
    const char* sql = "SELECT COUNT(*) FROM MATCHES;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Statement error on getting total matches: \n";
        return -1;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        totalMatches = sqlite3_column_int(stmt,0);
        sqlite3_finalize(stmt);
        return totalMatches;
    }
    sqlite3_finalize(stmt);
    return -1;
}



void insertMove (const string moveMade, const float timeToMove, const int move_counter, const Color moveMadeBy, const long long matchId) {
    char* errMsg;
    string color = moveMadeBy == Color::WHITE ? "White" : "Black";
    const char* sql = "INSERT INTO MATCH_MOVES(MATCH_ID, MOVE, MOVE_NUMBER, TIME_TO_MOVE, PLAYER) "
                      "VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Statement error on inserting move\n";
        return;
    }
    if (sqlite3_bind_int64(stmt, 1, matchId) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, moveMade.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, move_counter) != SQLITE_OK ||
        sqlite3_bind_double(stmt, 4, timeToMove) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 5, color.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK
        ) {
        cout << "Failed to bind variables: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Error occurred executing insert: " << sqlite3_errmsg(db) << "\n";
    } else {
        cout << "Move insert successful!\n";
    }
    sqlite3_finalize(stmt);
}

vector<string> getMatchMoves(long long matchId) {
    vector<string> matchMoves;
    const char* sql = "SELECT * FROM MATCH_MOVES WHERE MATCH_ID = ?";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Statement error on getting match moves\n";
        return matchMoves;
    }
    if (sqlite3_bind_int64(stmt, 1, matchId) != SQLITE_OK) {
        cout << "Failed to bind variables(line 1287): " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return matchMoves;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 2);
        matchMoves.push_back(text ? (const char*)text : "Unknown");
    }
    sqlite3_finalize(stmt);
    return matchMoves;
}

void getAllMatchMoves() {
    const char* sql = "SELECT MOVE FROM MATCH_MOVES";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Statement error on getting match moves\n";
        return ;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        cout << (text ? (const char*)text : "Unknown") << endl;
    }
    sqlite3_finalize(stmt);
}

int main() {

    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    #endif
    const char* filename = "db.sqlite3";
    openDB(filename);
    initialiseDB();
    //insertData();
    //getData();
    cout << "Welcome! What would you like to do?\n\n";
    while (true) {
        int userInput;
        cout << "Select an option(1-3)\n";
        cout << "1. Play \n2. Match History \n3. Exit \n4. Functionalities to be added\n";
        cin >> userInput;
        if (userInput == 4) {
            cout << "if ambitious enough - GUI made in SDL3? Website?\n";
            cout << "En Passant, Castling, Pawn promotion\n";
            cout << "if not using GUI - colors to indicate \033[3;104;30mValid moves\033[0m\t\t\n";
            cout << "multiplayer\n";
            cout << endl;
        }
        else if (userInput == 3) {
            sqlite3_close(db);
            return 0;
        }
        else if (userInput == 2) {
            int offset = 0;
            int limit = 10;
            while (true) {
                int totalMatches = getTotalMatches();
                cout << "Select match(0-9) or page(P0-P" << totalMatches / 10 << ") - 'E' to exit\n";
                cout << "------ START ------\n";
                vector<tuple<long long, string, string, string, string> > matches = getMatchIds(offset, limit);
                if (!matches.empty()) {
                    for (int i = 0; i < matches.size(); i++) {
                        cout << i << ". ";
                        cout << get<2>(matches[i]) << " VS. " << get<3>(matches[i]) << " - ";
                        cout << get<4>(matches[i]) << " || " << get<1>(matches[i]) << endl;
                    }
                }
                cout << "------ END ------\n";


                char historyUserInput[10];
                bool page = false;

                cin >> historyUserInput;
                //input -> selectare pagina
                if (tolower(historyUserInput[0]) == 'p') {
                    strcpy(historyUserInput, historyUserInput + 1);
                    page = true;
                }

                //transformare input in int
                int number_input;
                if (tolower(historyUserInput[0] == 'e' && strlen(historyUserInput) == 1))
                    break;
                bool ok = true;
                if (strlen(historyUserInput) == 0) {
                    ok = false;
                }

                for (int i = 0; i < strlen(historyUserInput); i++) {
                    if (!isdigit(historyUserInput[i])) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    number_input = stoi(historyUserInput);

                    //selectare meci
                    if (!page && number_input >= 0 && number_input < matches.size()) {
                        int matchToGet = number_input;
                        cout << "Getting info about match " << matchToGet << "...\n\n";
                        cout << "------ START ------\n";
                        vector<string> matchMoves = getMatchMoves(get<0>(matches[matchToGet]));
                        for (int i = 0; i < matchMoves.size(); i++) {
                            cout << i + 1 << ". " << matchMoves[i] << endl;
                        }
                        cout << "------ END ------\n\n";
                        break;
                    }
                    //selectare pagina
                    if (page && number_input <= totalMatches / 10 && number_input >= 0) {
                        offset = number_input * limit;
                    }
                    if (number_input == -1) {
                        cout << "Going back...\n";
                        break;
                    }
                    if (!cin) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input invalid\n";
                    }
                }
                else {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Input invalid\n";
                }
            }
        } else if (userInput == 1) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            Board chessboard;
            gameEngine Engine;

            //cout << endl;


            int move_counter = 0;


            Engine.setPlayerName();
            long long matchId = insertMatch(Engine.getPlayerName(Color::WHITE), Engine.getPlayerName(Color::BLACK));
            std::chrono::steady_clock::time_point turnStartTime = std::chrono::steady_clock::now();
            chessboard.showChessboard();
            while (true) {
                //creare meci
                //insertMatch()
                float *durations = Engine.getMoveDuration();

                if (Engine.playerMove(chessboard, turnStartTime)) {
                    //inserare miscare & reset timer
                    turnStartTime = std::chrono::steady_clock::now();
                    insertMove(Engine.getMove(move_counter), durations[move_counter], move_counter,
                               Engine.getCurrentTurn(), matchId);
                    move_counter++;
                }


                vector<tuple<char, bool, Position, Position> > history = Engine.getMoveHistory();
                for (int i = 0; i < history.size(); i++) {
                    cout << "Move #" << i + 1 << ": "
                            << history[i]
                            << " | Duration: " << durations[i] << "s" << endl;
                }

                cout << Engine.getLastMove()[0] << " ";
                cout << Engine.getLastMove()[1] << endl;

                if (chessboard.isKingInCheck(Engine.getCurrentTurn())) {
                    cout << Engine.getCurrentTurn() << " is checked\n";
                    if (chessboard.isCheckmate(Engine.getCurrentTurn())) {
                        if (Engine.getCurrentTurn() == Color::WHITE) {
                            cout << "BLACK WINS!!!\n";
                            insertWin(matchId, 1);
                        } else if (Engine.getCurrentTurn() == Color::BLACK) {
                            cout << "WHITE WINS!!!\n";
                            insertWin(matchId, 0);
                        }
                        break;
                    }
                } else {
                    if (chessboard.isCheckmate(Engine.getCurrentTurn())) {
                        cout << "DRAW";
                        insertWin(matchId, -1);
                        break;
                    }
                }
            }
            sqlite3_close(db);
            return 0;
        } else {
            if (!cin) {
                cin.clear();
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input invalid\n";
        }
    }
    return 0;
}
