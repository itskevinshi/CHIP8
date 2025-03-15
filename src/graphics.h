//
// Created by kevin on 3/7/2025.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>

#define COLS 64
#define ROWS 32
#define SCALE_FACTOR 10
#define WINDOW_TITLE "Chip8 Emulator"

class chip8;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

void setupGraphics();
void drawGraphics(chip8 chip8Instance);

#endif //GRAPHICS_H