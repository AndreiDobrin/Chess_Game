//
// Created by Andrei on 4/27/2026.
//

#ifndef CHESS_GAME_GLOBALS_H
#define CHESS_GAME_GLOBALS_H
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

extern int kScreenWidth;
extern int kScreenHeight;
extern float xOffset;
extern float yOffset;

/* Global Variables */
//The window we'll be rendering to
extern SDL_Window* window;

//The renderer used to draw to the window
extern SDL_Renderer* renderer;

//The surface contained by the window
extern SDL_Surface* gScreenSurface;

//The image we will load and show on the screen
extern SDL_Surface* gHelloWorld;




#endif //CHESS_GAME_GLOBALS_H