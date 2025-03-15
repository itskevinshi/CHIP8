//
// Created by kevin on 3/7/2025.
//

#include "graphics.h"
#include "chip8.h"
#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void setupGraphics()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        WINDOW_TITLE,
        COLS * SCALE_FACTOR,
        ROWS * SCALE_FACTOR,
        SDL_WINDOW_OPENGL
    );


    if (window == nullptr)
    {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr)
    {
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}

void drawGraphics(const chip8& chip8Instance)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if (SDL_RenderClear(renderer) != true)
    {
        std::cerr << "Clear error: " << SDL_GetError() << std::endl;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int row = 0; row < 32; row++)
    {
        for (int col = 0; col < 64; col++)
        {
            if (chip8Instance.gfx[row * 64 + col] == 1)
            {
                SDL_FRect rect;
                rect.x = col * SCALE_FACTOR;
                rect.y = row * SCALE_FACTOR;
                rect.w = SCALE_FACTOR;
                rect.h = SCALE_FACTOR;
                if (SDL_RenderFillRect(renderer, &rect) != true) {
                    std::cerr << "Fill error: " << SDL_GetError() << std::endl;
                }            }
        }
    }
    SDL_RenderPresent(renderer);
}