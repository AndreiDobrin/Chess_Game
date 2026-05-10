
#include "UI.h"
#include "Globals.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include <cstdio>
using namespace std;


// UI::UI() {
// }

bool UI::setPointingCursor() {
    return SDL_SetCursor(pointingCursor);
}
bool UI::setDefaultCursor() {
    return SDL_SetCursor(defaultCursor);
}

bool UI::Init() {
    // 1. Initialize SDL Video Subsystem
    // In SDL3, functions return a boolean (false on failure)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // 2. Initialize the TTF Engine
    if (!TTF_Init()) {
        cerr << "TTF could not initialize! Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return false;
    }

    // 2. Create Window
    if (!window) {
        std::cerr << "Window could not be created! Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // 3. Create Renderer
    // NULL tells SDL to pick the best graphics driver automatically
    if (!renderer) {
        std::cerr << "Renderer could not be created! Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // 4. Load the Font (Make sure arial.ttf is in the same folder as your .exe)
    font = TTF_OpenFont("arial.ttf", 28);
    if (!font) {
        cerr << "Failed to load font! Did you put arial.ttf in the right folder? Error: " << SDL_GetError() << endl;
        return false;
    }

    // ENABLE ALPHA BLENDING
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetCursor(defaultCursor);
    buildPlayBtn();
    
    // Initialise DB
    database.initialiseDB();
    
    return true;
}

int UI::Menu() {
            bool playHovered = false;
            bool historyHovered = false;

            // CHECK FOR USER INPUT
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    return -1;
                }

                if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                }

                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    if (buttonClicked(playButton, mouseX, mouseY)) {
                        return 1;
                    }
                    if (buttonClicked(historyButton, mouseX, mouseY)) {
                        historyOffset = 0;
                        return 2; // Transition to History
                    }
                }
            }

            playHovered = buttonClicked(playButton, mouseX, mouseY);
            historyHovered = buttonClicked(historyButton, mouseX, mouseY);

            // Drawing
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
            SDL_RenderClear(renderer);

            // Draw Play Button
            SDL_SetRenderDrawColor(renderer, playHovered ? 255 : 255, playHovered ? 0 : 255, playHovered ? 0 : 255, 255);
            SDL_RenderFillRect(renderer, &playButton);
            SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);

            // Draw History Button
            SDL_SetRenderDrawColor(renderer, historyHovered ? 255 : 255, historyHovered ? 0 : 255, historyHovered ? 0 : 255, 255);
            SDL_RenderFillRect(renderer, &historyButton);
            
            float hW, hH;
            SDL_GetTextureSize(historyTextTexture, &hW, &hH);
            SDL_FRect hTextRect = { historyButton.x + (historyButton.w - hW)/2.0f, historyButton.y + (historyButton.h - hH)/2.0f, hW, hH };
            SDL_RenderTexture(renderer, historyTextTexture, nullptr, &hTextRect);

            if (playHovered || historyHovered) setPointingCursor();
            else setDefaultCursor();

            SDL_RenderPresent(renderer);
            return 0;

}

int UI::History() {
    bool backHovered = false;
    bool prevHovered = false;
    bool nextHovered = false;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) return -1;
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (buttonClicked(backButton, mouseX, mouseY)) return 0;
            if (buttonClicked(prevButton, mouseX, mouseY) && historyOffset >= historyLimit) historyOffset -= historyLimit;
            if (buttonClicked(nextButton, mouseX, mouseY)) {
                if (historyOffset + historyLimit < database.getTotalMatches()) historyOffset += historyLimit;
            }
        }
    }

    backHovered = buttonClicked(backButton, mouseX, mouseY);
    prevHovered = buttonClicked(prevButton, mouseX, mouseY);
    nextHovered = buttonClicked(nextButton, mouseX, mouseY);

    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_RenderClear(renderer);

    // Draw Back Button
    SDL_SetRenderDrawColor(renderer, backHovered ? 200 : 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &backButton);
    float bW, bH;
    SDL_GetTextureSize(backTextTexture, &bW, &bH);
    SDL_FRect bTRect = { backButton.x + (backButton.w - bW)/2, backButton.y + (backButton.h - bH)/2, bW, bH };
    SDL_RenderTexture(renderer, backTextTexture, nullptr, &bTRect);

    // Fetch matches
    auto matches = database.getMatchIds(historyOffset, historyLimit);
    float startY = 100;
    for (int i = 0; i < matches.size(); i++) {
        char matchStr[200];
        // format: White VS Black - Result || Date
        sprintf(matchStr, "%d. %s VS %s - %s", historyOffset + i + 1, get<2>(matches[i]).c_str(), get<3>(matches[i]).c_str(), get<4>(matches[i]).c_str());
        
        SDL_Surface* s = TTF_RenderText_Blended(font, matchStr, 0, {255, 255, 255, 255});
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        float tw, th;
        SDL_GetTextureSize(t, &tw, &th);
        SDL_FRect r = { 50, startY, tw, th };
        SDL_RenderTexture(renderer, t, nullptr, &r);
        startY += th + 10;
        SDL_DestroySurface(s);
        SDL_DestroyTexture(t);
    }

    // Draw Prev/Next
    if (historyOffset >= historyLimit) {
        SDL_SetRenderDrawColor(renderer, prevHovered ? 200 : 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &prevButton);
        float pW, pH;
        SDL_GetTextureSize(prevTextTexture, &pW, &pH);
        SDL_FRect pTRect = { prevButton.x + (prevButton.w - pW)/2, prevButton.y + (prevButton.h - pH)/2, pW, pH };
        SDL_RenderTexture(renderer, prevTextTexture, nullptr, &pTRect);
    }

    if (historyOffset + historyLimit < database.getTotalMatches()) {
        SDL_SetRenderDrawColor(renderer, nextHovered ? 200 : 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &nextButton);
        float nW, nH;
        SDL_GetTextureSize(nextTextTexture, &nW, &nH);
        SDL_FRect nTRect = { nextButton.x + (nextButton.w - nW)/2, nextButton.y + (nextButton.h - nH)/2, nW, nH };
        SDL_RenderTexture(renderer, nextTextTexture, nullptr, &nTRect);
    }

    if (backHovered || prevHovered || nextHovered) setPointingCursor();
    else setDefaultCursor();

    SDL_RenderPresent(renderer);
    return 2;
}

bool UI::buttonClicked(SDL_FRect button, float mouseX, float mouseY) {
    if (mouseX >= button.x && mouseX <= (button.x + button.w) &&
                    mouseY >= button.y && mouseY <= (button.y + button.h)) {
        return true;
                    }
    return false;
}

void UI::buildPlayBtn() {
    textColor = { 0, 0, 0, 255 }; // Black text
    playButton = { (kScreenWidth - menuButtonW) / 2.0f, (kScreenHeight - menuButtonH) / 2.0f - 100, menuButtonW, menuButtonH };

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Play", 4, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    // History Button
    historyButton = { playButton.x, playButton.y + 70, menuButtonW, menuButtonH };
    textSurface = TTF_RenderText_Blended(font, "History", 7, textColor);
    historyTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    // Back Button
    backButton = { 20, 20, 100, 40 };
    textSurface = TTF_RenderText_Blended(font, "Back", 4, textColor);
    backTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    // Prev Button
    prevButton = { (kScreenWidth / 2.0f) - 110, (float)kScreenHeight - 60, 100, 40 };
    textSurface = TTF_RenderText_Blended(font, "Prev", 4, textColor);
    prevTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    // Next Button
    nextButton = { (kScreenWidth / 2.0f) + 10, (float)kScreenHeight - 60, 100, 40 };
    textSurface = TTF_RenderText_Blended(font, "Next", 4, textColor);
    nextTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    // Forfeit Button
    forfeitButton = { (float)kScreenWidth - 150, (float)kScreenHeight / 2.0f - 20, 130, 40 };
    textSurface = TTF_RenderText_Blended(font, "Forfeit", 7, textColor);
    forfeitTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    // 7. Center the Text inside the Button
    float textW, textH;
    SDL_GetTextureSize(textTexture, &textW, &textH);
    textRect.w = textW;
    textRect.h = textH;
    textRect.x = playButton.x + (playButton.w - textW) / 2.0f; // Center X
    textRect.y = playButton.y + (playButton.h - textH) / 2.0f; // Center Y
}

UI::~UI() {
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(historyTextTexture);
    SDL_DestroyTexture(backTextTexture);
    SDL_DestroyTexture(prevTextTexture);
    SDL_DestroyTexture(nextTextTexture);
    
    TTF_CloseFont(font);
    TTF_Quit(); // Quit TTF

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void UI::renderTimer(SDL_Renderer* renderer, float seconds, float x, float y, Color color) {
    int minutes = (int)seconds / 60;
    int secs = (int)seconds % 60;
    char timeStr[10];
    sprintf(timeStr, "%02d:%02d", minutes, secs);

    SDL_Color colorSDL = (color == Color::WHITE) ? SDL_Color{255, 255, 255, 255} : SDL_Color{0, 0, 0, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, timeStr, 0, colorSDL);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    SDL_FRect rect = {x, y, w, h};
    
    // Draw background for timer
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 180);
    SDL_FRect bgRect = {x - 5, y - 5, w + 10, h + 10};
    SDL_RenderFillRect(renderer, &bgRect);
    
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
    
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

int UI::Game(Board& chessboard, GameEngine& engine, bool& gameInit, Position& pieceSelected, int& i, int& j,
             vector<Position>& wantedValidPositions, int& m, int& n) {
    if (!gameInit) {
        gameInit = true;
        engine.reset();
        initChessboardLayout();
        chessboard.setupBoard();
        engine.startMatch(database);
    }

    // Update timers
    engine.updateTimers();

    // Check for timer expiration
    if (engine.getTimeLeft(Color::WHITE) <= 0) {
        cout << "White's time is up! BLACK WINS!!!" << endl;
        database.insertWin(engine.getCurrentMatchId(), 1);
        gameInit = false; // Reset for next game
        return 0; // Back to menu
    }
    if (engine.getTimeLeft(Color::BLACK) <= 0) {
        cout << "Black's time is up! WHITE WINS!!!" << endl;
        database.insertWin(engine.getCurrentMatchId(), 0);
        gameInit = false; // Reset for next game
        return 0; // Back to menu
    }

    // CHECK FOR USER INPUT
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return -1;
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            m = (mouseY - yOffset) / 50;
            n = (mouseX - xOffset) / 50;
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            // CLICKED ON PROMOTION MENU
            if (isPromoting) {
                int selection = -1;
                for (int k = 0; k < 4; k++) {
                    if (buttonClicked(promotionMenuRects[k], mouseX, mouseY)) {
                        selection = k;
                        break;
                    }
                }

                if (selection != -1) {
                    unique_ptr<Piece> newPiece;
                    if (selection == 0) newPiece = make_unique<Queen>(promotionColor);
                    else if (selection == 1) newPiece = make_unique<Rook>(promotionColor);
                    else if (selection == 2) newPiece = make_unique<Bishop>(promotionColor);
                    else if (selection == 3) newPiece = make_unique<Knight>(promotionColor);

                    chessboard.replacePiece(promotionPos, std::move(newPiece));
                    isPromoting = false;

                    engine.switchTurn();
                    Color nextTurn = engine.getCurrentTurn();
                    if (chessboard.isKingInCheck(nextTurn)) {
                        cout << (nextTurn == Color::WHITE ? "White" : "Black") << " is checked" << endl;
                        if (chessboard.isCheckmate(nextTurn)) {
                            cout << (nextTurn == Color::WHITE ? "BLACK" : "WHITE") << " WINS!!!" << endl;
                            database.insertWin(engine.getCurrentMatchId(), (nextTurn == Color::WHITE ? 1 : 0));
                        }
                    } else {
                        if (chessboard.isCheckmate(nextTurn)) {
                            cout << "DRAW (Stalemate)" << endl;
                            database.insertWin(engine.getCurrentMatchId(), -1);
                        }
                    }
                }
                continue; // Skip board logic if promoting
            }

            // CLICKED ON GRID
            if (mouseX >= get<0>(chessboardGrid[0][0]) && mouseY >= get<1>(chessboardGrid[0][0]) &&
                mouseX <= get<0>(chessboardGrid[7][7]) + 50 && mouseY <= get<1>(chessboardGrid[7][7]) + 50) {
                i = (mouseY - yOffset) / 50;
                j = (mouseX - xOffset) / 50;

                // PIECE ALREADY SELECTED
                if (pieceSelected.col != -1 && pieceSelected.row != -1) {
                    if ((chessboard.getPositionInfo({i, j}) != nullptr &&
                         chessboard.getPositionInfo({i, j})->getColor() != engine.getCurrentTurn()) ||
                        chessboard.getPositionInfo({i, j}) == nullptr) {
                        
                        bool validMove = false;
                        for (const auto& pos : wantedValidPositions) {
                            if (j == pos.col && i == pos.row) {
                                validMove = true;
                                break;
                            }
                        }

                        if (validMove) {
                            Position from = pieceSelected;
                            Position to = {i, j};
                            Color moverColor = engine.getCurrentTurn();
                            
                            // Check if capture
                            bool isCapture = (chessboard.getPositionInfo(to) != nullptr);
                            char pieceChar = chessboard.getPositionInfo(from)->identifyPiece()[0];
                            
                            static auto turnStartTime = std::chrono::steady_clock::now();
                            if (chessboard.movePiece(from, to, moverColor)) {
                                auto moveEndTime = std::chrono::steady_clock::now();
                                float duration = std::chrono::duration<float>(moveEndTime - turnStartTime).count();
                                turnStartTime = moveEndTime;

                                // Record in engine
                                engine.recordMove(pieceChar, isCapture, from, to, duration);
                                
                                // Log move to DB with algebraic notation
                                database.insertMove(engine.getMove(engine.getMoveHistorySize() - 1), duration, engine.getMoveHistorySize(), moverColor, engine.getCurrentMatchId());

                                // Cerinta 6: Downcasting sigur cu dynamic_cast pentru promovare
                                // Justificare: getPositionInfo returneaza un Piece*, dar canPromote() este specifica clasei Pawn.
                                // dynamic_cast este preferat fata de static_cast deoarece verifica la runtime tipul real al obiectului.
                                // Daca am folosi static_cast pe o piesa care nu e Pion, am avea Undefined Behavior.
                                // dynamic_cast returneaza nullptr daca piesa nu este un Pion, permitand o tratare sigura.
                                Pawn* pawn = dynamic_cast<Pawn*>(chessboard.getPositionInfo({i, j}));
                                if (pawn != nullptr && pawn->canPromote({i, j})) {
                                    isPromoting = true;
                                    promotionPos = {i, j};
                                    promotionColor = pawn->getColor();
                                    float startX = get<0>(chessboardGrid[i][j]);
                                    float startY = get<1>(chessboardGrid[i][j]);
                                    if (startY > (float)kScreenHeight / 2.0f) startY -= 150.0f;
                                    for (int k = 0; k < 4; k++) {
                                        promotionMenuRects[k] = {startX, startY + (float)k * 50.0f, 50.0f, 50.0f};
                                    }
                                } else {
                                    engine.switchTurn();
                                    Color nextTurn = engine.getCurrentTurn();
                                    if (chessboard.isKingInCheck(nextTurn)) {
                                        cout << (nextTurn == Color::WHITE ? "White" : "Black") << " is checked" << endl;
                                        if (chessboard.isCheckmate(nextTurn)) {
                                            cout << (nextTurn == Color::WHITE ? "BLACK" : "WHITE") << " WINS!!!" << endl;
                                            database.insertWin(engine.getCurrentMatchId(), (nextTurn == Color::WHITE ? 1 : 0));
                                        }
                                    } else if (chessboard.isCheckmate(nextTurn)) {
                                        cout << "DRAW (Stalemate)" << endl;
                                        database.insertWin(engine.getCurrentMatchId(), -1);
                                    }
                                }
                            }
                            pieceSelected = {-1, -1};
                            continue;
                        }
                    }
                }

                // SELECT PIECE
                if (chessboard.getPositionInfo({i, j}) != nullptr &&
                    chessboard.getPositionInfo({i, j})->getColor() == engine.getCurrentTurn()) {
                    pieceSelected = {i, j};
                    wantedValidPositions = chessboard.getPositionInfo({i, j})->getValidMoves(chessboard, {i, j});
                } else {
                    pieceSelected = {-1, -1};
                }
            }

            // CLICKED ON FORFEIT
            if (buttonClicked(forfeitButton, mouseX, mouseY)) {
                Color winner = (engine.getCurrentTurn() == Color::WHITE ? Color::BLACK : Color::WHITE);
                cout << (engine.getCurrentTurn() == Color::WHITE ? "White" : "Black") << " forfeited! " 
                     << (winner == Color::WHITE ? "WHITE" : "BLACK") << " WINS!!!" << endl;
                database.insertWin(engine.getCurrentMatchId(), (winner == Color::WHITE ? 0 : 1));
                gameInit = false;
                return 0; // Back to menu
            }
        }
    }

    // DRAWING
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_RenderClear(renderer);

    if (pieceSelected.col != -1 && pieceSelected.row != -1) {
        chessboard.drawChessBoard(renderer, {pieceSelected.row, pieceSelected.col}, wantedValidPositions);
    } else {
        chessboard.drawChessboard(renderer, chessboard);
    }

    // Render timers
    renderTimer(renderer, engine.getTimeLeft(Color::WHITE), 20, kScreenHeight - 60, Color::WHITE);
    renderTimer(renderer, engine.getTimeLeft(Color::BLACK), 20, 20, Color::BLACK);

    // Render Forfeit Button
    bool forfeitHovered = buttonClicked(forfeitButton, mouseX, mouseY);
    SDL_SetRenderDrawColor(renderer, forfeitHovered ? 200 : 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &forfeitButton);
    float fW, fH;
    SDL_GetTextureSize(forfeitTextTexture, &fW, &fH);
    SDL_FRect fTRect = { forfeitButton.x + (forfeitButton.w - fW)/2.0f, forfeitButton.y + (forfeitButton.h - fH)/2.0f, fW, fH };
    SDL_RenderTexture(renderer, forfeitTextTexture, nullptr, &fTRect);

    if (isPromoting) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_FRect dimRect = {0, 0, (float)kScreenWidth, (float)kScreenHeight};
        SDL_RenderFillRect(renderer, &dimRect);
        for (int k = 0; k < 4; k++) {
            SDL_SetRenderDrawColor(renderer, buttonClicked(promotionMenuRects[k], mouseX, mouseY) ? 200 : 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &promotionMenuRects[k]);
            unique_ptr<Piece> temp;
            if (k == 0) temp = make_unique<Queen>(promotionColor);
            else if (k == 1) temp = make_unique<Rook>(promotionColor);
            else if (k == 2) temp = make_unique<Bishop>(promotionColor);
            else if (k == 3) temp = make_unique<Knight>(promotionColor);
            SDL_RenderTexture(renderer, temp->getImg(), nullptr, &promotionMenuRects[k]);
        }
    }

    // CURSOR LOGIC
    bool isOverBoard = (mouseX >= get<0>(chessboardGrid[0][0]) && mouseY >= get<1>(chessboardGrid[0][0]) &&
                       mouseX <= get<0>(chessboardGrid[7][7]) + 50 && mouseY <= get<1>(chessboardGrid[7][7]) + 50);
    bool isHoveringOwnPiece = !isPromoting && isOverBoard && m >= 0 && m < 8 && n >= 0 && n < 8 &&
                             chessboard.getPositionInfo({m,n}) != nullptr &&
                             chessboard.getPositionInfo({m,n})->getColor() == engine.getCurrentTurn();
    bool isHoveringSelection = false;
    if (isPromoting) {
        for (int k = 0; k < 4; k++) if (buttonClicked(promotionMenuRects[k], mouseX, mouseY)) isHoveringSelection = true;
    } else {
        for (const auto& x : wantedValidPositions) {
            if (isOverBoard && m == x.row && n == x.col && pieceSelected.row != -1) {
                isHoveringSelection = true;
                break;
            }
        }
    }

    if ((isHoveringOwnPiece || isHoveringSelection)) setPointingCursor();
    else setDefaultCursor();

    SDL_RenderPresent(renderer);
    return 1;
}


void UI::initChessboardLayout() {
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
