//
// Created by Andrei on 4/27/2026.
//
#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include "Board.h"

class GameDB;


class GameEngine {
    Color currentTurn;
    // piesa mutata; captureaza piesa?; from; to.
    std::vector<std::tuple<char, bool, Position, Position>> moveHistory;
    float* moveDurations;
    int movesCapacity;
    char whitePlayer[21];
    char blackPlayer[21];
    
    float whiteTimeLeft;
    float blackTimeLeft;
    long long currentMatchId;
    std::chrono::steady_clock::time_point lastTick;

    void expandCapacity();
public:
    GameEngine();

    ~GameEngine();
    GameEngine(GameEngine &other);

    bool playerMove(Board& board, std::chrono::steady_clock::time_point startTime);
    void switchTurn();
    
    void updateTimers();
    float getTimeLeft(Color color) const;
    void setInitialTime(float minutes);
    void startMatch(GameDB& db);
    long long getCurrentMatchId() const;
    void recordMove(char piece, bool capture, Position from, Position to, float duration);
    void reset();

    Color getCurrentTurn();
    std::vector<std::tuple<char, bool, Position, Position>> getMoveHistory();
    float* getMoveDuration();
    int getMoveHistorySize();
    void setPlayerName();
    const char* getPlayerName(Color color);
    std::string getMove(int i);
    std::vector<Position> getLastMove();

};