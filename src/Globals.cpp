//
// Created by Andrei on 4/27/2026.
//

#include "Globals.h"

int kScreenWidth{ 640 };
int kScreenHeight{ 480 };

/* Global Variables */
//The window we'll be rendering to
SDL_Window* window = SDL_CreateWindow("SDL3 Hello World", kScreenWidth, kScreenHeight, 0);

//The renderer used to draw to the window
SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

//The surface contained by the window
SDL_Surface* gScreenSurface{ nullptr };

//The image we will load and show on the screen
SDL_Surface* gHelloWorld{ nullptr };

