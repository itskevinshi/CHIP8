#include <iostream>
#include "chip8.h"
#include "graphics.h"

chip8 myChip8;

int main(int argc, char **argv)
{
    setupGraphics();
    myChip8.initialize();
    myChip8.loadApplication("C:/Users/kevin/Documents/UVA/Coding Projects/CHIP8/tests/roms/invaders.c8");

    SDL_Event event {};
    bool quit = false;

    // Timer variables
    static float timer_accumulator = 0.0f;
    const float TIMER_INTERVAL = 16.666667f; // 60Hz precise
    static Uint32 last_timer_update = SDL_GetTicks();

    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            myChip8.setKeys(&event);
        }

        myChip8.emulateCycle();

        if (myChip8.drawFlag) {
            drawGraphics(myChip8);
            SDL_Delay(16);
            myChip8.drawFlag = false;
            std::cout << "drawing done..." << std::endl;
        }

        // Timer handling using accumulator pattern
        Uint32 current_time = SDL_GetTicks();
        float elapsed = static_cast<float>(current_time - last_timer_update);
        last_timer_update = current_time;
        timer_accumulator += elapsed;

        while(timer_accumulator >= TIMER_INTERVAL) {
            myChip8.updateTimers();
            timer_accumulator -= TIMER_INTERVAL;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}