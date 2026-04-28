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
#include "Piece.h"
#include "Color.h"
#include "Position.h"
#include "Pawn.h"
#include "Board.h"
#include "Rook.h"
#include "Knight.h"
#include "GameDB.h"
#include "GameEngine.h"
#include "UI.h"
#include "Window.h"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"




class Board;
using namespace std;

std::ostream& operator<< (std::ostream& os, const std::tuple<char, bool, Position, Position>& moveHistory)  { // supraincarcare << pentru moveHistory
    if (std::get<0>(moveHistory) != 'P')
        os << std::get<0>(moveHistory);
    if (std::get<1>(moveHistory))
        os << 'x';
    os << char(tolower(char(std::get<3>(moveHistory).col + 'A'))) << std::get<3>(moveHistory).row + 1;
    return os;
}

std::ostream& operator<< (std::ostream& os, const std::vector<std::tuple<char, bool, Position, Position>>& moveHistory) { // supraincarcare << pentru moveHistory
    for (int i = 0; i < moveHistory.size(); i++) {
        os << i + 1 << ".: ";// << get<0>(moveHistory[i]);
        if (std::get<0>(moveHistory[i]) != 'P')
            os << std::get<0>(moveHistory[i]);
        if (std::get<1>(moveHistory[i]))
            os << 'x';
        os << char(tolower(char(std::get<3>(moveHistory[i]).col + 'A'))) << std::get<3>(moveHistory[i]).row + 1 << "\n";
    }
    return os;
}

// bool loadMedia()
// {
//     //File loading flag
//     bool success{ true };
//
//     //Load splash image
//     if( gPngTexture.loadFromFile( "./src/hello-sdl3.bmp" ) == false )
//     {
//         SDL_Log( "Unable to load png image!\n");
//         success = false;
//     }
//
//     return success;
// }



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

bool buttonClicked(SDL_FRect button, float mouseX, float mouseY) {
    if (mouseX >= button.x && mouseX <= (button.x + button.w) &&
                    mouseY >= button.y && mouseY <= (button.y + button.h)) {
        return true;
    }


    return false;
}

vector<vector<tuple<float, float>>> chessboardGrid;

void initChessboard() {

    float xOffset = (float(kScreenWidth) - 50.0f*8)/2;
    float yOffset = (float(kScreenHeight) - 50.0f*8)/2;

    SDL_FRect rect = {xOffset, yOffset, 50.0f, 50.0f};
    for (int i = 0; i < 8; i++) {
        vector<tuple<float, float>> chessLine;
        for (int j = 0; j < 8; j++) {
            rect.x = xOffset + 50.0f * float(j);
            rect.y = yOffset + 50.0f * float(i);
            // cout << rect.x << endl << rect.y;
            chessLine.push_back(make_tuple(rect.x, rect.y));
        }
        chessboardGrid.push_back(chessLine);
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << "["<< i << "][" << j << "]: "<<get<0>(chessboardGrid[i][j]) << " " << get<1>(chessboardGrid[i][j]) << endl;
        }
    }
    // << get<0>(chessboardGrid[0][0]) << " " << get<1>(chessboardGrid[0][0]) << endl;
    //cout << get<0>(chessboardGrid[7][7]) << " " << get<1>(chessboardGrid[7][7]) << endl;
}

float xOffset = (float(kScreenWidth) - 50.0f*8)/2;
float yOffset = (float(kScreenHeight) - 50.0f*8)/2;

// void drawChessboard(SDL_Renderer* renderer, Board& board) {
//
//
//
//     SDL_FRect rect = {xOffset, yOffset, 50.0f, 50.0f};
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             rect.x = xOffset + 50.0f * j;
//             rect.y = yOffset + 50.0f * i;
//             if ((i + j) % 2 == 0) {
//                 SDL_SetRenderDrawColor(renderer, 90, 56, 47, 255);
//             }
//             else {
//                 SDL_SetRenderDrawColor(renderer, 149, 83, 59, 255);
//             }
//             SDL_RenderFillRect(renderer, &rect);
//             if (board.getPositionInfo({i,j}) != nullptr) {
//                 SDL_RenderTexture(renderer, board.getPositionInfo({i,j})->getImg(), nullptr, &rect);
//             }
//         }
//     }
// }

// void quitGame(bool& isRunning) {
//
// }

int main() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
    //GameDB database;


    // 1. Initialize SDL Video Subsystem
    // In SDL3, functions return a boolean (false on failure)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Initialize the TTF Engine
    if (TTF_Init() == -1) {
        cerr << "TTF could not initialize! Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    // 2. Create Window
    if (!window) {
        std::cerr << "Window could not be created! Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 3. Create Renderer
    // NULL tells SDL to pick the best graphics driver automatically
    if (!renderer) {
        std::cerr << "Renderer could not be created! Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 4. Load the Font (Make sure arial.ttf is in the same folder as your .exe)
    TTF_Font* font = TTF_OpenFont("arial.ttf", 28);
    if (!font) {
        cerr << "Failed to load font! Did you put arial.ttf in the right folder? Error: " << SDL_GetError() << endl;
    }

    SDL_Cursor* handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);

    SDL_Cursor* defaultCursor = SDL_GetDefaultCursor();

    // 4. Main Event Loop
    bool isRunning = true;
    SDL_Event event;

    // BUTTON AREA
    float menuButtonW = 200;
    float menuButtonH = 50;
    // BUTTON TEXT COLOR
    SDL_Color textColor = { 0, 0, 0, 255 }; // Black text
    SDL_FRect playButton = { (kScreenWidth - menuButtonW) / 2.0f, (kScreenHeight - menuButtonH) / 2.0f - 100, menuButtonW, menuButtonH };

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Play", 4, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface); // We don't need the surface anymore

    // 7. Center the Text inside the Button
    SDL_FRect textRect;
    float textW, textH;
    SDL_GetTextureSize(textTexture, &textW, &textH);
    textRect.w = textW;
    textRect.h = textH;
    textRect.x = playButton.x + (playButton.w - textW) / 2.0f; // Center X
    textRect.y = playButton.y + (playButton.h - textH) / 2.0f; // Center Y

    float mouseX = -1;
    float mouseY = -1;

    int state = 0;
    bool gameInit = false;
    Board chessboard;
    int i = 0;
    int j = 0;
    vector<Position> wantedValidPositions;
    Position pieceSelected = {-1, -1};
    GameEngine engine;

    while (isRunning) {
        if (state == 0) {
            bool buttonHovered = false;

            // CHECK FOR USER INPUT
            while (SDL_PollEvent(&event)) {
                // SDL_EVENT_QUIT means the user clicked the 'X' button on the window
                if (event.type == SDL_EVENT_QUIT) {
                    isRunning = false;
                }

                // MEMORISE MOUSE POSITIONS FOR HOVER
                if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                }

                // CHECK FOR CLICKS
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    // float mouseX = event.button.x;
                    // float mouseY = event.button.y;

                    // PLAY BUTTON CLICK
                    if (buttonClicked(playButton, mouseX, mouseY)) {
                        cout << "Play Button was clicked!" << endl;
                        // START GAME
                        bool gameInit = false;
                        state = 1;
                    }
                    cout << mouseX << " " << mouseY << endl;
                }
            }

            if (mouseX >= playButton.x && mouseX <= (playButton.x + playButton.w) &&
                mouseY >= playButton.y && mouseY <= (playButton.y + playButton.h)) {

                buttonHovered = true;
                }

            // 5. Drawing
            // Set the background color to a nice blue (Red, Green, Blue, Alpha)
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
            SDL_RenderClear(renderer); // Wipe the screen with the blue color

            // DRAW BUTTON
            if (buttonHovered) {
                SDL_SetCursor(handCursor);
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            }
            else {
                SDL_SetCursor(defaultCursor);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_RenderFillRect(renderer, &playButton);
            SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);



            // Show what we've drawn on the screen
            SDL_RenderPresent(renderer);
        }

        // PLAY LOGIC
        else if (state == 1) {
            if (!gameInit) {
                gameInit = true;
                initChessboard();
                chessboard.setupBoard();
            }

            // RESET CURSOR
            SDL_SetCursor(defaultCursor);

            // CHECK FOR USER INPUT
            while (SDL_PollEvent(&event)) {
                // SDL_EVENT_QUIT means the user clicked the 'X' button on the window
                if (event.type == SDL_EVENT_QUIT) {
                    isRunning = false;
                }

                // MEMORISE MOUSE POSITIONS FOR HOVER
                if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;

                }
                // CHECK FOR CLICKS
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    // float mouseX = event.button.x;
                    // float mouseY = event.button.y;

                    // CLICKED ON GRID
                    if (mouseX >= get<0>(chessboardGrid[0][0]) && mouseY >= get<1>(chessboardGrid[0][0]) &&
                        mouseX <= get<0>(chessboardGrid[7][7]) + 50 && mouseY <= get<1>(chessboardGrid[7][7]) + 50) {
                        i = (mouseY - yOffset) / 50;
                        j = (mouseX - xOffset) / 50;
                        cout << "Chessboard Grid clicked: " << i << " " << j << endl;

                        // PIECE ALREADY SELECTED
                        if (pieceSelected.col != -1 && pieceSelected.row != -1) {
                            for (int k = 0; k <= wantedValidPositions.size(); k++) {
                                cout << wantedValidPositions[k] << " ";
                                if (j == wantedValidPositions[k].col &&
                                    i == wantedValidPositions[k].row) {
                                    cout << "VALID\n";
                                    cout<<chessboard.movePiece(pieceSelected, {i, j}, engine.getCurrentTurn());
                                    break;
                                    }
                            }
                            pieceSelected = {-1, -1};
                            i = -1;
                            j = -1;
                        }
                        // PIECE NOT SELECTED
                        else {

                            //
                            if (chessboard.getPositionInfo({i,j}) != nullptr) {
                            pieceSelected = {i, j};
                            wantedValidPositions = chessboard.getPositionInfo({i,j})->getValidMoves(chessboard, {i,j});
                            // for (int k = 0; k < wantedValidPositions.size(); k++) {
                            //     cout << wantedValidPositions[k] << endl;
                            // }
                            }
                            else {
                                pieceSelected = {-1, -1};
                            }
                            cout << "Piece selected: " << pieceSelected << endl;
                        }
                    }


                    // // BACK BUTTON CLICK
                    // if (buttonClicked(backButton, mouseX, mouseY)) {
                    //     cout << "Back Button was clicked!" << endl;
                    //     // BACK TO MENU
                    //     state = 0;
                    // }
                    cout << mouseX << " " << mouseY << endl;
                }
            }



            // Set the background color to a nice blue (Red, Green, Blue, Alpha)
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
            SDL_RenderClear(renderer); // Wipe the screen with the blue color

            //initChessboard();
            if (pieceSelected.col != -1 && pieceSelected.row != -1) {
                chessboard.drawChessBoard(renderer, {i,j}, wantedValidPositions);
            }
            else {
                chessboard.drawChessboard(renderer, chessboard);
            }

            if (mouseX >= get<0>(chessboardGrid[0][0]) && mouseY >= get<1>(chessboardGrid[0][0]) &&
                mouseX <= get<0>(chessboardGrid[7][7])+ 50 && mouseY <= get<1>(chessboardGrid[7][7]) + 50) {

                SDL_SetCursor(handCursor);
            }
            else {
                SDL_SetCursor(defaultCursor);
            }


            SDL_RenderPresent(renderer);
        }
    }

    // 6. Cleanup (Always clean up your pointers!)

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit(); // Quit TTF

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}






    // cout << "Welcome! What would you like to do?\n\n";
    // while (true) {
    //     int userInput;
    //     cout << "Select an option(1-3)\n";
    //     cout << "1. Play \n2. Match History \n3. Exit \n4. Functionalities to be added\n";
    //     cin >> userInput;
    //     if (userInput == 4) {
    //         cout << "if ambitious enough - GUI made in SDL3? Website?\n";
    //         cout << "En Passant, Castling, Pawn promotion\n";
    //         cout << "if not using GUI - colors to indicate \033[3;104;30mValid moves\033[0m\t\t\n";
    //         cout << "multiplayer\n";
    //         cout << endl;
    //     }
    //     else if (userInput == 3) {
    //         database.closeDB();
    //         return 0;
    //     }
    //     else if (userInput == 2) {
    //         int offset = 0;
    //         int limit = 10;
    //         while (true) {
    //             int totalMatches = database.getTotalMatches();
    //             cout << "Select match(0-9) or page(P0-P" << totalMatches / 10 << ") - 'E' to exit\n";
    //             cout << "------ START ------\n";
    //             vector<tuple<long long, string, string, string, string> > matches = database.getMatchIds(offset, limit);
    //             if (!matches.empty()) {
    //                 for (int i = 0; i < matches.size(); i++) {
    //                     cout << i << ". ";
    //                     cout << get<2>(matches[i]) << " VS. " << get<3>(matches[i]) << " - ";
    //                     cout << get<4>(matches[i]) << " || " << get<1>(matches[i]) << endl;
    //                 }
    //             }
    //             cout << "------ END ------\n";
    //
    //
    //             char historyUserInput[10];
    //             bool page = false;
    //
    //             cin >> historyUserInput;
    //             //input -> selectare pagina
    //             if (tolower(historyUserInput[0]) == 'p') {
    //                 strcpy(historyUserInput, historyUserInput + 1);
    //                 page = true;
    //             }
    //
    //             //transformare input in int
    //             int number_input;
    //             if (tolower(historyUserInput[0] == 'e' && strlen(historyUserInput) == 1))
    //                 break;
    //             bool ok = true;
    //             if (strlen(historyUserInput) == 0) {
    //                 ok = false;
    //             }
    //
    //             for (int i = 0; i < strlen(historyUserInput); i++) {
    //                 if (!isdigit(historyUserInput[i])) {
    //                     ok = false;
    //                     break;
    //                 }
    //             }
    //             if (ok) {
    //                 number_input = stoi(historyUserInput);
    //
    //                 //selectare meci
    //                 if (!page && number_input >= 0 && number_input < matches.size()) {
    //                     int matchToGet = number_input;
    //                     cout << "Getting info about match " << matchToGet << "...\n\n";
    //                     cout << "------ START ------\n";
    //                     vector<string> matchMoves = database.getMatchMoves(get<0>(matches[matchToGet]));
    //                     for (int i = 0; i < matchMoves.size(); i++) {
    //                         cout << i + 1 << ". " << matchMoves[i] << endl;
    //                     }
    //                     cout << "------ END ------\n\n";
    //                     break;
    //                 }
    //                 //selectare pagina
    //                 if (page && number_input <= totalMatches / 10 && number_input >= 0) {
    //                     offset = number_input * limit;
    //                 }
    //                 if (number_input == -1) {
    //                     cout << "Going back...\n";
    //                     break;
    //                 }
    //                 if (!cin) {
    //                     cin.clear();
    //                     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //                     cout << "Input invalid\n";
    //                 }
    //             }
    //             else {
    //                 cin.clear();
    //                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //                 cout << "Input invalid\n";
    //             }
    //         }
    //     } else if (userInput == 1) {
    //         cin.clear();
    //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //         Board chessboard;
    //         GameEngine Engine;
    //
    //         //cout << endl;
    //
    //
    //         int move_counter = 0;
    //
    //
    //         Engine.setPlayerName();
    //         long long matchId = database.insertMatch(Engine.getPlayerName(Color::WHITE), Engine.getPlayerName(Color::BLACK));
    //         std::chrono::steady_clock::time_point turnStartTime = std::chrono::steady_clock::now();
    //         chessboard.showChessboard();
    //         while (true) {
    //             //creare meci
    //             //insertMatch()
    //             float *durations = Engine.getMoveDuration();
    //
    //             if (Engine.playerMove(chessboard, turnStartTime)) {
    //                 //inserare miscare & reset timer
    //                 turnStartTime = std::chrono::steady_clock::now();
    //                 database.insertMove(Engine.getMove(move_counter), durations[move_counter], move_counter,
    //                            Engine.getCurrentTurn(), matchId);
    //                 move_counter++;
    //             }
    //
    //
    //             vector<tuple<char, bool, Position, Position> > history = Engine.getMoveHistory();
    //             for (int i = 0; i < history.size(); i++) {
    //                 cout << "Move #" << i + 1 << ": "
    //                         << history[i]
    //                         << " | Duration: " << durations[i] << "s" << endl;
    //             }
    //
    //             cout << Engine.getLastMove()[0] << " ";
    //             cout << Engine.getLastMove()[1] << endl;
    //
    //             if (chessboard.isKingInCheck(Engine.getCurrentTurn())) {
    //                 cout << Engine.getCurrentTurn() << " is checked\n";
    //                 if (chessboard.isCheckmate(Engine.getCurrentTurn())) {
    //                     if (Engine.getCurrentTurn() == Color::WHITE) {
    //                         cout << "BLACK WINS!!!\n";
    //                         database.insertWin(matchId, 1);
    //                     } else if (Engine.getCurrentTurn() == Color::BLACK) {
    //                         cout << "WHITE WINS!!!\n";
    //                         database.insertWin(matchId, 0);
    //                     }
    //                     break;
    //                 }
    //             } else {
    //                 if (chessboard.isCheckmate(Engine.getCurrentTurn())) {
    //                     cout << "DRAW";
    //                     database.insertWin(matchId, -1);
    //                     break;
    //                 }
    //             }
    //         }
    //         database.closeDB();
    //         return 0;
    //     } else {
    //         if (!cin) {
    //             cin.clear();
    //         }
    //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //         cout << "Input invalid\n";
    //     }
    // }
