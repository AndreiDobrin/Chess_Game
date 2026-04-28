//
// Created by Andrei on 4/27/2026.
//

#pragma once
#include "sqlite3.h"
#include "Position.h"
#include <iostream>
#include <tuple>
#include <vector>
#include "Color.h"


class GameDB {
    sqlite3* db;
    const char* filename = "db.sqlite3";
public:

    GameDB();

    void openDB(const char* filename);
    void initialiseDB();
    void insertData();
    void getData();
    //friend std::ostream& operator<< (std::ostream& os, const std::tuple<char, bool, Position, Position>& moveHistory);
    //friend std::ostream& operator<< (std::ostream& os, const std::vector<std::tuple<char, bool, Position, Position>>& moveHistory);
    long long insertMatch(const char* whitePlayer, const char* blackPlayer);
    void insertWin(long long matchId, int result);
    std::vector<std::tuple<long long, std::string, std::string, std::string, std::string>> getMatchIds(int offset, int limit);
    int getTotalMatches();
    void insertMove (std::string moveMade, float timeToMove, int move_counter, Color moveMadeBy, long long matchId);
    std::vector<std::string> getMatchMoves(long long matchId);
    void getAllMatchMoves();
    void closeDB();
};


