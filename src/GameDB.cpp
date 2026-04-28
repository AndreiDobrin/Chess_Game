//
// Created by Andrei on 4/27/2026.
//

#include "GameDB.h"
using namespace std;

GameDB::GameDB() {
    openDB(filename);
    initialiseDB();
}

void GameDB::openDB(const char* filename) {
    int opened = sqlite3_open(filename, &db);
    if (opened) {
        printf("Database could not be opened %s\n", sqlite3_errmsg(db));
    } else {
        //printf("Opened database successfully\n");
    }
}

void GameDB::initialiseDB() {
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

// std::ostream& operator<< (std::ostream& os, const std::tuple<char, bool, Position, Position>& moveHistory)  { // supraincarcare << pentru moveHistory
//     if (std::get<0>(moveHistory) != 'P')
//         os << std::get<0>(moveHistory);
//     if (std::get<1>(moveHistory))
//         os << 'x';
//     os << char(tolower(char(std::get<3>(moveHistory).col + 'A'))) << std::get<3>(moveHistory).row + 1;
//     return os;
// }
//
// std::ostream& operator<< (std::ostream& os, const std::vector<std::tuple<char, bool, Position, Position>>& moveHistory) { // supraincarcare << pentru moveHistory
//     for (int i = 0; i < moveHistory.size(); i++) {
//         os << i + 1 << ".: ";// << get<0>(moveHistory[i]);
//         if (std::get<0>(moveHistory[i]) != 'P')
//             os << std::get<0>(moveHistory[i]);
//         if (std::get<1>(moveHistory[i]))
//             os << 'x';
//         os << char(tolower(char(std::get<3>(moveHistory[i]).col + 'A'))) << std::get<3>(moveHistory[i]).row + 1 << "\n";
//     }
//     return os;
// }

void GameDB::insertData() {
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

void GameDB::getData() {
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



long long GameDB::insertMatch(const char* whitePlayer, const char* blackPlayer) {
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "INSERT INTO MATCHES(WHITE, BLACK) VALUES(?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Error inserting match in database\n";
        return -1;
    }
    if (sqlite3_bind_text(stmt, 1, whitePlayer, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, blackPlayer, -1, SQLITE_TRANSIENT) != SQLITE_OK
        ) {

        std::cout << "Failed to bind variables: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return -1;
        }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return -1;
    }

    std::cout << "Match inserted in database successfully\n";
    sqlite3_finalize(stmt);
    return sqlite3_last_insert_rowid(db);
}

void GameDB::insertWin(long long matchId, int result) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE TABLE MATCHES"
                      "SET RESULT = ?"
                      "WHERE MATCH_ID = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Error inserting win in database\n";
        return;
    }
    std::string resultText;
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
        std::cout << "Failed to bind variables(line 1177): " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
        }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }
    std::cout << "Match result inserted in db successfully\n";
    sqlite3_finalize(stmt);
}

vector<tuple<long long, string, string, string, string>> GameDB::getMatchIds(int offset, int limit) {
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


int GameDB::getTotalMatches() {
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

void GameDB::insertMove (const string moveMade, const float timeToMove, const int move_counter, const Color moveMadeBy, const long long matchId) {
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

vector<string> GameDB::getMatchMoves(long long matchId) {
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

void GameDB::getAllMatchMoves() {
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

void GameDB::closeDB() {
    sqlite3_close(db);
}
