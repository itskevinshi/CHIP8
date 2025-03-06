//
// Created by kevin on 2/18/2025.
//

#include "chip8.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <random>

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
    unsigned char part1 = memory[pc];
    unsigned char part2 = memory[pc + 1];
    unsigned short opcode = part1 << 8 | part2;

    switch(opcode & 0xF000)
    {
    case 0x0000:
        switch(opcode & 0x00FF)
        {
            case 0x00E0:
                for (int i = 0; i < sizeof(gfx) / sizeof(gfx[0]); i++)
                {
                    gfx[i] = 0;
                }
                drawFlag = true;
                pc += 2;
                break;

            case 0x00EE: // Return from subroutine
                // implement 00EE
                // decrease stack pointer
                if (sp > 0)
                {
                    sp--;
                    // set program counter to value at the top of the stack
                    pc = stack[sp];
                    pc += 2;
                }
                break;

            default: // 0NNN: Calls machine code routine (RCA 1802)
                // implement 0NNN
                break;
        }
        break;

    case 0x1000:
        pc = opcode & 0x0FFF;
        break;

    case 0x2000:
        if (sp < 16)
        {
            pc = stack[sp];
            sp++;
            pc = opcode & 0x0FFF;
        }
        break;

    case 0x3000:
        {
            // skip the next instruction if register v[x] is equal to NN
            const unsigned char vReg = V[(opcode & 0x0F00) >> 8];
            if (const unsigned char NN = opcode & 0x00FF; vReg == NN)
            {
                pc += 4;
            }
            else
            {
                pc += 2;
            }
            break;
        }

    case 0x4000:
        {
            // skip the next instruction if register v[x] is not equal to NN
            const unsigned char vReg = V[(opcode & 0x0F00) >> 8];
            if (const unsigned char NN = opcode & 0x00FF; vReg != NN)
            {
                pc += 4;
            }
            else
            {
                pc += 2;
            }
            break;
        }

    case 0x5000:
        {
            if (V[opcode & 0x0F00 >> 8] == V[opcode & 0x00F0 >> 4])
            {
                pc += 4;
            }
            else
            {
                pc += 2;
            }
            break;
        }

    case 0x6000:
        {
            const unsigned char NN = opcode & 0x00FF;
            V[(opcode & 0x0F00) >> 8] = NN;
            pc += 2;
            break;
        }

    case 0x7000:
        {
            const unsigned char NN = opcode & 0x00FF;
            V[(opcode & 0x0F00) >> 8] += NN;
            pc += 2;
            break;
        }

    case 0x8000:
        switch (opcode & 0x000F)
        {
            case 0x0000:
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
            case 0x0001:
                {
                    const unsigned char bitOr = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = bitOr;
                    pc += 2;
                    break;
                }
            case 0x0002:
                {
                    const unsigned char bitAnd = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = bitAnd;
                    pc += 2;
                    break;
                }

            case 0x0003:
                {
                    const unsigned char bitXOR = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = bitXOR;
                    pc += 2;
                    break;
                }

            case 0x0004:
                {
                    const unsigned char sumResult = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = sumResult;
                    // check for overflow
                    if (V[(opcode & 0x0F00) >> 8] < sumResult || V[(opcode & 0x00F0) >> 4] < sumResult)
                    {
                        V[15] = 1;
                    }
                    else
                    {
                        V[15] = 0;
                    }
                    pc += 2;
                    break;
                }
            case 0x0005:
                {
                    const unsigned char minusResult = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = minusResult;
                    // set flag if underflow
                    unsigned char flag = 0;
                    if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                    {
                        flag = 1;
                    }
                    V[15] = flag;
                    pc += 2;
                    break;
                }

            case 0x0006:
                {
                    V[15] = V[(opcode & 0x0F00) >> 8] & 1;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
                    pc += 2;
                    break;
                }

            case 0x0007:
                {
                    const unsigned char minusResult = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
                    {
                        V[15] = 1;
                    }
                    else
                    {
                        V[15] = 0;
                    }
                    pc += 2;
                    break;
                }

            case 0x000E:
                {
                    V[15] = (V[(opcode & 0x0F00) >> 8] >> 7) & 1;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
                    pc += 2;
                    break;
                }
        }

    case 0x9000:
        // skip next instruction if VX != VY
        switch (opcode & 0x000F)
        {
            case 0x0000:
                if (V[opcode & 0x0F00 >> 8] != V[(opcode & 0x00F0 >> 4)])
                    {
                        pc += 4;
                    }
                else
                    {
                        pc += 2;
                    }
                break;
        }

    case 0xA000:
        I = opcode & 0x0FFF;
        pc += 2;
        break;

    case 0xB000:
        pc = V[0] + opcode & 0x0FFF;
        break;

    case 0xC000:
        {
            // Seed for random number generation
            std::random_device rd;
            std::mt19937 gen(rd());

            // Define the range
            int min_val = 0;
            int max_val = 255;

            // Distribution
            std::uniform_int_distribution<> distrib(min_val, max_val);
            unsigned char randNum = distrib(gen);
            V[opcode & 0x0F00 >> 8] = randNum & (opcode & 0x00FF);
            pc += 2;
            break;
        }

    case 0xD000:
        {
            unsigned char xCoord = V[(opcode & 0x0F00) >> 8] % 64;
            unsigned char yCoord = V[(opcode & 0x00F0) >> 4] % 32;
            V[15] = 0;
            unsigned char n = opcode & 0x000F;
            for (int i = 0; i < n; i++)
            {
                unsigned char spriteByte = memory[I+i];
                for (int j = 0; j < 8; j++)
                {
                    unsigned char bit = (spriteByte >> 7-j) & 0x01;
                    if (xCoord + j >= 64)
                    {
                        break;
                    }
                    if (bit == 1 && gfx[yCoord * 64 + xCoord + j] == 1)
                    {
                        gfx[yCoord * 64 + xCoord + j] = 0;
                        V[15] = 1;
                    }
                    else if (bit == 1 && gfx[yCoord * 64 + xCoord + j] == 0)
                    {
                        gfx[yCoord * 64 + xCoord + j] = 1;
                    }
                }
                yCoord++;
                if (yCoord >= 32)
                {
                    break;
                }
            }
            drawFlag = true;
            pc += 2;
            break;
        }
    }
}

void chip8::debugRender() const
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
        std::cerr << "Failed to open file " << filename << std::endl;
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