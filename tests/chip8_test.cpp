//
// Created by kevin on 2/20/2025.
//
#include <gtest/gtest.h>
#include "../src/chip8.h"

class Chip8Test : public testing::Test {
protected:
    chip8 myChip8;

    void SetIRegister() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0xA2;
        myChip8.memory[0x201] = 0x10;

        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.I, 528);
    }

    void ClearScreenOpcode() {
        myChip8.initialize();

        // Set some pixels
        myChip8.gfx[0] = 1;
        myChip8.gfx[10] = 1;

        // Load clear screen opcode (00E0)
        myChip8.memory[0x200] = 0x00;
        myChip8.memory[0x201] = 0xE0;

        myChip8.emulateCycle();

        // Verify screen is cleared
        for(int i = 0; i < 64 * 32; i++) {
            EXPECT_EQ(myChip8.gfx[i], 0);
        }
    }

    void TestDebugRender() {
        myChip8.initialize();
        myChip8.gfx[0] = 1;
        myChip8.gfx[10] = 1;
        for (int i = 0; i < 64; i++) {
            myChip8.gfx[2 * 64 + i] = 1;
        }
    }

    void TestLoadPong() {
        myChip8.initialize();
        unsigned char firstSixteenBytesOfPong [16] = {0x22, 0xfc, 0x6b, 0x0c, 0x6c, 0x3f, 0x6d, 0x0c, 0xa2, 0xea, 0xda, 0xb6, 0xdc, 0xd6, 0x6e, 0x00};
        unsigned char lastSixteenBytesOfPong [16] = {0x7a, 0x08, 0x3a, 0x40, 0x13, 0x12, 0xa2, 0xf6, 0x6a, 0x00, 0x6b, 0x20, 0xdb, 0xa1, 0x00, 0xee};
        int PONGFILESIZE = 294;

        for (int i = 512; i < 4096; i++) {
            EXPECT_EQ(myChip8.memory[i], 0);
        }

        EXPECT_TRUE(myChip8.loadApplication("C:/Users/kevin/Documents/UVA/Coding Projects/CHIP8/tests/roms/pong2.c8"));

        for (int i = 512; i < 528; i++) {
            EXPECT_EQ(myChip8.memory[i], firstSixteenBytesOfPong[i - 512]);
            EXPECT_EQ(myChip8.memory[i + PONGFILESIZE - 16], lastSixteenBytesOfPong[i - 512]);
        }

        for (int i = 512 + PONGFILESIZE; i < 4096; i++) {
            EXPECT_EQ(myChip8.memory[i], 0);
        }
    }
};

TEST_F(Chip8Test, ClearScreenOpcode) {
    ClearScreenOpcode();
}

TEST_F(Chip8Test, SetIRegister) {
    SetIRegister();
}

TEST_F(Chip8Test, TestDebugRender) {
    TestDebugRender();
    // myChip8.debugRender();
}

TEST_F(Chip8Test, LoadPong) {
    TestLoadPong();
}
