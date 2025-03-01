//
// Created by kevin on 2/18/2025.
//
#pragma once
class chip8 {
public:
    chip8();
    ~chip8();

    bool drawFlag;

    void initialize();

    void emulateCycle();
    void debugRender() const;
    bool loadApplication(const char * filename);

    // Chip8
    unsigned char  gfx[64 * 32] = {0x00};	// Total amount of pixels: 2048
    unsigned char  key[16] = {0x00};

private:
    unsigned short pc;				// Program counter
    unsigned short opcode;			// Current opcode
    unsigned short I;				// Index register
    unsigned short sp;				// Stack pointer

    unsigned char  V[16] = {0x00};			// V-regs (V0-VF)
    unsigned short stack[16] = {0};		// Stack (16 levels)
    unsigned char  memory[4096] = {0x00};	// Memory (size = 4k)

    unsigned char  delay_timer;		// Delay timer
    unsigned char  sound_timer;		// Sound timer
    
    friend class Chip8Test; // Single friend declaration for test fixture
};
