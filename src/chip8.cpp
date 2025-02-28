//
// Created by kevin on 2/18/2025.
//

#include "chip8.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>

unsigned char chip8_fontset[80]
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

chip8::chip8()
{
// empty
}

chip8::~chip8()
{
// empty
}


void chip8::initialize()
{
    pc = 0x200; // program counter starts at 0x200
    opcode = 0; // reset current opcode
    I = 0; // reset index register
    sp = 0; // reset stack pointer

    // clear display
    for (int i = 0; i < sizeof(gfx) / sizeof(gfx[0]); i++)
    {
        gfx[i] = 0;
    }
    // clear stack
    for (int i = 0; i < sizeof(stack) / sizeof(stack[0]); i++)
    {
        stack[i] = 0;
    }
    // clear registors V0-VF
    for (int i = 0; i < sizeof(V) / sizeof(V[0]); i++)
    {
        V[i] = 0;
    }
    // clear memory
    for (int i = 0; i < sizeof(memory) / sizeof(memory[0]); i++)
    {
        memory[i] = 0;
    }
    // load fontset
    for (int i = 0; i < 80; i++)
    {
        memory[i] = chip8_fontset[i];
    }
    // reset timers
    delay_timer = 0;
    sound_timer = 0;
}

void chip8::emulateCycle()
{
    // Fetch opcode - this part is correct
    unsigned char part1 = memory[pc];
    unsigned char part2 = memory[pc + 1];
    unsigned short opcode = part1 << 8 | part2;
    pc += 2;

    switch(opcode & 0xF000) // Only check the first nibble initially
    {
    case 0x0000:
        switch(opcode & 0x00FF) // For 0xxx opcodes, check last byte
        {
            case 0x00E0: // Clear screen
                for (int i = 0; i < sizeof(gfx) / sizeof(gfx[0]); i++)
                {
                    gfx[i] = 0;
                }
                drawFlag = true;
                break;

            case 0x00EE: // Return from subroutine
                // implement 00EE
                break;

            default: // 0NNN: Calls machine code routine (RCA 1802)
                // implement 0NNN
                break;
        }
        break;

    case 0xA000:
        I = opcode & 0x0FFF;
        break;

    }
}

void chip8::debugRender()
{
    constexpr int ROWS {32};
    constexpr int COLS {64};
    std::cout << std::endl;
    for (int i = 0; i < ROWS; i++) {
        std::string row;
        for (int j = 0; j < COLS; j++) {
            if (gfx[i * COLS + j] == 0) row += "0 ";
            else row += "1 ";
        }
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

bool chip8::loadApplication(const char * filename)
{
    std::ifstream input(filename, std::ios::in | std::ios::binary);

    if (!input.is_open())
    {
        return false;
    }

    input.seekg(0, std::ios::beg);

    int startpos {512};
    while (!input.eof())
    {
        int byte = input.get();
        if (byte == EOF) {
            break;
        }
        memory[startpos] = byte;
        startpos++;
    }

    input.close();
    return true;
}