#include <SDL.h>
#include <iostream>
#include "chip8.hpp"  // Your cpu core implementation

/* Setting Globals */
chip8 myChip8;
const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 320;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* gameSurface = NULL;
SDL_Renderer* renderer = NULL;

bool init() {
    bool successfulInit = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize: " << SDL_GetError()
                  << std::endl;
        successfulInit = false;
    } else {
        window = SDL_CreateWindow("Chippy-Emu", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            std::cerr << "Window could be created: " << SDL_GetError()
                      << std::endl;
            successfulInit = false;
        } else {
            screenSurface = SDL_GetWindowSurface(window);
        }
    }

    return successfulInit;
}

bool loadMedia() {
    bool successfulInit = true;

    return successfulInit;
}

void close() {
    SDL_FreeSurface(screenSurface);
    screenSurface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void drawGraphics() {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 48; j++) {
            if (myChip8.gfx[i*64 + j]) {
                SDL_Rect pixel = {i*10, j*10, 10, 10};
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderClear(renderer);  
                SDL_RenderFillRect( renderer, &pixel );
            }
        }
    }
    SDL_BlitSurface(gameSurface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);
}

int main(int argv, char** args) {
    init();

    // Fill the surface white
    SDL_FillRect(screenSurface, NULL,
                 SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

    // Update the surface
    SDL_UpdateWindowSurface(window);

    close();
    // Set up render system and register input callbacks
    // setupGraphics();
    // setupInput();

    // Initialize the Chip8 system and load the game into the memory
    // myChip8.initialize();
    // myChip8.loadGame("pong");

    // Emulation loop
    // for (;;) {
    // Emulate one cycle
    // myChip8.emulateCycle();

    // If the draw flag is set, update the screen
    // if (myChip8.drawFlag)
    //    drawGraphics();

    // Store key press state (Press and Release)
    // myChip8.setKeys();
    //}

    return 0;
}