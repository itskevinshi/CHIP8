#include "chip8.h"
#include "graphics.h"

chip8 myChip8;

int main(int argc, char **argv)
{
    setupGraphics();

    myChip8.initialize();
    // myChip8.loadApplication("C:/Users/kevin/Documents/UVA/Coding Projects/CHIP8/tests/roms/pong2.c8");
    // myChip8.loadApplication("C:/Users/kevin/Documents/UVA/Coding Projects/CHIP8/tests/roms/logo.ch8");
    myChip8.loadApplication("C:/Users/kevin/Documents/UVA/Coding Projects/CHIP8/tests/roms/IBM Logo.ch8");

    SDL_Event event {};
    bool quit = false;

    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            myChip8.setKeys(&event);
        }

        myChip8.emulateCycle();
        if (myChip8.drawFlag) {
            drawGraphics(myChip8);
            myChip8.drawFlag = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}