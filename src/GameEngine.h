//
// Created by Andrei on 4/27/2026.
//
#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include "Board.h"


class GameEngine {
    Color currentTurn;
    // piesa mutata; captureaza piesa?; from; to.
    std::vector<std::tuple<char, bool, Position, Position>> moveHistory;
    float* moveDurations;
    int movesCapacity;
    char whitePlayer[21];
    char blackPlayer[21];

    void expandCapacity();
public:
    GameEngine();

    ~GameEngine();
    GameEngine(GameEngine &other);

    bool playerMove(Board& board, std::chrono::steady_clock::time_point startTime);

    Color getCurrentTurn();
    std::vector<std::tuple<char, bool, Position, Position>> getMoveHistory();
    float* getMoveDuration();
    int getMoveHistorySize();
    void setPlayerName();
    const char* getPlayerName(Color color);
    std::string getMove(int i);
    std::vector<Position> getLastMove();

};