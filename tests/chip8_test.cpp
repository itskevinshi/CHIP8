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
};

TEST_F(Chip8Test, ClearScreenOpcode) {
    ClearScreenOpcode();
}

TEST_F(Chip8Test, SetIRegister) {
    SetIRegister();
}
