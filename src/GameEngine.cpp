#include <cstring>
#include <limits>
#include "GameEngine.h"
#include "GameDB.h"
using namespace std;

    void GameEngine::expandCapacity() {
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
    GameEngine::GameEngine() {
        currentTurn = Color::WHITE;
        movesCapacity = 10;
        moveDurations = new float[movesCapacity];
        strcpy(whitePlayer, "Guest");
        strcpy(blackPlayer, "Guest");
        whiteTimeLeft = 600.0f; // 10 minutes
        blackTimeLeft = 600.0f;
        currentMatchId = -1;
        lastTick = std::chrono::steady_clock::now();
    }

    GameEngine::~GameEngine() {
        delete[] moveDurations;
    };
    GameEngine::GameEngine(GameEngine &other) {
        this->currentTurn = other.currentTurn;
        this->moveHistory = other.moveHistory;
        this->movesCapacity = other.movesCapacity;
        this->moveDurations = new float[this->movesCapacity];
        this->whiteTimeLeft = other.whiteTimeLeft;
        this->blackTimeLeft = other.blackTimeLeft;
        this->currentMatchId = other.currentMatchId;
        this->lastTick = other.lastTick;
        strcpy(this->whitePlayer, other.whitePlayer);
        strcpy(this->blackPlayer, other.blackPlayer);

        for (int i = 0; i < this->movesCapacity; i++) {
            this->moveDurations[i] = other.moveDurations[i];
        }
    }

    void GameEngine::switchTurn() {
        if (currentTurn == Color::WHITE) {
            currentTurn = Color::BLACK;
        }
        else {
            currentTurn = Color::WHITE;
        }
        lastTick = std::chrono::steady_clock::now();
    }

    void GameEngine::updateTimers() {
        auto now = std::chrono::steady_clock::now();
        float delta = std::chrono::duration<float>(now - lastTick).count();
        lastTick = now;

        if (currentTurn == Color::WHITE) {
            whiteTimeLeft -= delta;
            if (whiteTimeLeft < 0) whiteTimeLeft = 0;
        } else {
            blackTimeLeft -= delta;
            if (blackTimeLeft < 0) blackTimeLeft = 0;
        }
    }

    float GameEngine::getTimeLeft(Color color) const {
        return (color == Color::WHITE) ? whiteTimeLeft : blackTimeLeft;
    }

    void GameEngine::setInitialTime(float minutes) {
        whiteTimeLeft = minutes * 60.0f;
        blackTimeLeft = minutes * 60.0f;
    }

    void GameEngine::startMatch(GameDB& db) {
        currentMatchId = db.insertMatch(whitePlayer, blackPlayer);
        lastTick = std::chrono::steady_clock::now();
    }

    long long GameEngine::getCurrentMatchId() const {
        return currentMatchId;
    }

    void GameEngine::recordMove(char piece, bool capture, Position from, Position to, float duration) {
        moveHistory.push_back(std::make_tuple(piece, capture, from, to));
        if (moveHistory.size() >= movesCapacity) {
            expandCapacity();
        }
        moveDurations[moveHistory.size() - 1] = duration;
    }

    bool GameEngine::playerMove(Board& board, std::chrono::steady_clock::time_point startTime) {
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

            switchTurn();

            board.showChessBoard(from, to);
            return true;
        }
        board.showChessboard();
        return false;
    }


    void GameEngine::reset() {
        currentTurn = Color::WHITE;
        whiteTimeLeft = 600.0f;
        blackTimeLeft = 600.0f;
        moveHistory.clear();
        currentMatchId = -1;
        lastTick = std::chrono::steady_clock::now();
    }

    Color GameEngine::getCurrentTurn() {
        return currentTurn;
    }
    vector<tuple<char, bool, Position, Position>> GameEngine::getMoveHistory() {
        return moveHistory;
    }
    float* GameEngine::getMoveDuration() {
        return moveDurations;
    }
    int GameEngine::getMoveHistorySize() {
        return moveHistory.size();
    }
    void GameEngine::setPlayerName() {
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
    const char* GameEngine::getPlayerName(Color color) {
        if (color == Color::WHITE)
            return whitePlayer;

        return blackPlayer;
    }

    string GameEngine::getMove(int i) {
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

    vector<Position> GameEngine::getLastMove() {
        return {get<2>(moveHistory[moveHistory.size() - 1]), get<3>(moveHistory[moveHistory.size() - 1])};
    }
