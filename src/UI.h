#pragma once
#include <SDL3/SDL.h>

#include "Board.h"
#include "GameEngine.h"
#include "Globals.h"
#include "myRectangle.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "GameDB.h"

class UI {
    SDL_Event event;
    GameDB database;

    SDL_Cursor* pointingCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    SDL_Cursor* defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);

    float mouseX = -1;
    float mouseY = -1;



    // FONT
    TTF_Font* font;

    // BUTTON AREA
    float menuButtonW = 200;
    float menuButtonH = 50;

    SDL_FRect playButton;
    SDL_FRect historyButton;
    SDL_FRect backButton;
    SDL_FRect prevButton;
    SDL_FRect nextButton;
    SDL_FRect forfeitButton;

    SDL_FRect textRect;
    SDL_Texture* textTexture;
    SDL_Texture* historyTextTexture;
    SDL_Texture* backTextTexture;
    SDL_Texture* prevTextTexture;
    SDL_Texture* nextTextTexture;
    SDL_Texture* forfeitTextTexture;

    SDL_Color textColor;

    // History state
    int historyOffset = 0;
    int historyLimit = 5;

    std::vector<std::vector<std::tuple<float, float>>> chessboardGrid;

    // Promotion UI state
    bool isPromoting = false;
    Color promotionColor;
    Position promotionPos = {-1, -1};
    SDL_FRect promotionMenuRects[4]; // Queen, Rook, Bishop, Knight
public:
    int Menu();
    int Game(Board& chessboard, GameEngine& engine, bool& gameInit, Position& pieceSelected, int& i, int& j,
             std::vector<Position>& wantedValidPositions, int& m, int& n);
    int History();
    bool buttonClicked(SDL_FRect button, float mouseX, float mouseY);
    bool Init();

    void buildPlayBtn();
    void initChessboardLayout();
    void renderTimer(SDL_Renderer* renderer, float seconds, float x, float y, Color color);

    bool setPointingCursor();
    bool setDefaultCursor();


    ~UI();
};

