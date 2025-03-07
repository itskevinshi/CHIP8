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

    void Opcode6XNNTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x6A;
        myChip8.memory[0x201] = 0x42;

        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0xA], 0x42);
        EXPECT_EQ(myChip8.pc, 0x202);
    }

    void Opcode7XNNTest() {
        myChip8.initialize();

        // First set V[2] to 5
        myChip8.memory[0x200] = 0x62;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x2], 5);
        EXPECT_EQ(myChip8.pc, 0x202);
    }

    void Opcode8XY0Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x65;
        myChip8.memory[0x201] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x8A;
        myChip8.memory[0x203] = 0x50;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0xA], 0x42);
        EXPECT_EQ(myChip8.V[0x5], 0x42);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void Opcode8XY1Test() {
        myChip8.initialize();

        // Set V[3] to 0x0F (0000 1111)
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x0F;
        myChip8.emulateCycle();

        // Set V[4] to 0xF0 (1111 0000)
        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0xF0;
        myChip8.emulateCycle();

        // Set V[3] to V[3] OR V[4]
        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x41;
        myChip8.emulateCycle();

        // Result should be 0xFF (1111 1111)
        EXPECT_EQ(myChip8.V[0x3], 0xFF);
        EXPECT_EQ(myChip8.V[0x4], 0xF0);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY2Test() {
        myChip8.initialize();

        // Set V[3] to 0x0F (0000 1111)
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x0F;
        myChip8.emulateCycle();

        // Set V[4] to 0xF3 (1111 0011)
        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0xF3;
        myChip8.emulateCycle();

        // Set V[3] to V[3] AND V[4]
        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x42;
        myChip8.emulateCycle();

        // Result should be 0x03 (0000 0011)
        EXPECT_EQ(myChip8.V[0x3], 0x03);
        EXPECT_EQ(myChip8.V[0x4], 0xF3);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY3Test() {
        myChip8.initialize();

        // Set V[3] to 0xFF (1111 1111)
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0xFF;
        myChip8.emulateCycle();

        // Set V[4] to 0xF0 (1111 0000)
        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0xF0;
        myChip8.emulateCycle();

        // Set V[3] to V[3] XOR V[4]
        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x43;
        myChip8.emulateCycle();

        // Result should be 0x0F (0000 1111)
        EXPECT_EQ(myChip8.V[0x3], 0x0F);
        EXPECT_EQ(myChip8.V[0x4], 0xF0);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY4Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0xFF;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x02;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x44;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x01);
        EXPECT_EQ(myChip8.V[0x4], 0x02);
        EXPECT_EQ(myChip8.V[0xF], 1);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY4NoCarryTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x44;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x0A);
        EXPECT_EQ(myChip8.V[0x4], 0x05);
        EXPECT_EQ(myChip8.V[0xF], 0);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY5Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x0A;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x45;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x05);
        EXPECT_EQ(myChip8.V[0x4], 0x05);
        EXPECT_EQ(myChip8.V[0xF], 1);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY5BorrowTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x0A;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x45;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0xFB);
        EXPECT_EQ(myChip8.V[0x4], 0x0A);
        EXPECT_EQ(myChip8.V[0xF], 0);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY6Test() {
        myChip8.initialize();

        // Set V[3] to 0x03 (0000 0011)
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x03;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x83;
        myChip8.memory[0x203] = 0x06;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x01);  // 0000 0011 -> 0000 0001
        EXPECT_EQ(myChip8.V[0xF], 1);     // LSB was 1
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void Opcode8XY6NoFlagTest() {
        myChip8.initialize();

        // Set V[3] to 0x02 (0000 0010)
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x02;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x83;
        myChip8.memory[0x203] = 0x06;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x01);  // 0000 0010 -> 0000 0001
        EXPECT_EQ(myChip8.V[0xF], 0);     // LSB was 0
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void Opcode8XY7Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x03;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x47;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x02);
        EXPECT_EQ(myChip8.V[0x4], 0x05);
        EXPECT_EQ(myChip8.V[0xF], 1);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XY7BorrowTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x03;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x83;
        myChip8.memory[0x205] = 0x47;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0xFE);
        EXPECT_EQ(myChip8.V[0x4], 0x03);
        EXPECT_EQ(myChip8.V[0xF], 0);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void Opcode8XYETest() {
        myChip8.initialize();

        // Set V[3] to 0x80 (1000 0000)
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x80;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x83;
        myChip8.memory[0x203] = 0x0E;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x00);
        EXPECT_EQ(myChip8.V[0xF], 1);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void Opcode8XYENoFlagTest() {
        myChip8.initialize();

        // Set V[3] to 0x40 (0100 0000)
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x40;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x83;
        myChip8.memory[0x203] = 0x0E;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x80);
        EXPECT_EQ(myChip8.V[0xF], 0);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void Opcode9XY0Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x43;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x93;
        myChip8.memory[0x205] = 0x40;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x42);
        EXPECT_EQ(myChip8.V[0x4], 0x43);
        EXPECT_EQ(myChip8.pc, 0x208);
    }

    void Opcode9XY0NoSkipTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x93;
        myChip8.memory[0x205] = 0x40;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x42);
        EXPECT_EQ(myChip8.V[0x4], 0x42);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void OpcodeANNNTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0xA1;
        myChip8.memory[0x201] = 0x23;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.I, 0x123);
        EXPECT_EQ(myChip8.pc, 0x202);

        myChip8.memory[0x202] = 0xAF;
        myChip8.memory[0x203] = 0xFF;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.I, 0xFFF);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void OpcodeBNNNTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x60;
        myChip8.memory[0x201] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xB1;
        myChip8.memory[0x203] = 0x00;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0], 0x42);
        EXPECT_EQ(myChip8.pc, 0x142);
    }

    void OpcodeCXNNTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0xC3;
        myChip8.memory[0x201] = 0xFF;
        myChip8.emulateCycle();

        EXPECT_GE(myChip8.V[0x3], 0x00);
        EXPECT_LE(myChip8.V[0x3], 0xFF);
        EXPECT_EQ(myChip8.pc, 0x202);

        myChip8.memory[0x202] = 0xC3;
        myChip8.memory[0x203] = 0x0F;
        myChip8.emulateCycle();

        EXPECT_GE(myChip8.V[0x3], 0x00);
        EXPECT_LE(myChip8.V[0x3], 0x0F);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void OpcodeDXYNTest() {
        myChip8.initialize();

        // Set V[3] (X coordinate) to 0
        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x00;
        myChip8.emulateCycle();

        // Set V[4] (Y coordinate) to 0
        myChip8.memory[0x202] = 0x64;
        myChip8.memory[0x203] = 0x00;
        myChip8.emulateCycle();

        // Store sprite data at a safe location (0x300)
        myChip8.memory[0x300] = 0xC0;  // 1100 0000
        myChip8.memory[0x301] = 0xC0;  // 1100 0000

        // Set I to point to memory location with sprite data
        myChip8.memory[0x204] = 0xA3;
        myChip8.memory[0x205] = 0x00;  // Set I to 0x300
        myChip8.emulateCycle();

        // Draw 2-byte sprite at (0,0)
        myChip8.memory[0x206] = 0xD3;
        myChip8.memory[0x207] = 0x42;
        myChip8.emulateCycle();

        // Verification steps remain the same
        EXPECT_EQ(myChip8.gfx[0], 1);
        EXPECT_EQ(myChip8.gfx[1], 1);
        EXPECT_EQ(myChip8.gfx[64], 1);
        EXPECT_EQ(myChip8.gfx[65], 1);
        EXPECT_EQ(myChip8.V[0xF], 0);  // No collision
        EXPECT_TRUE(myChip8.drawFlag);
        EXPECT_EQ(myChip8.pc, 0x208);

        // Draw again at same location to test collision
        myChip8.memory[0x208] = 0xD3;
        myChip8.memory[0x209] = 0x42;
        myChip8.emulateCycle();

        // Pixels should be flipped
        EXPECT_EQ(myChip8.gfx[0], 0);
        EXPECT_EQ(myChip8.gfx[1], 0);
        EXPECT_EQ(myChip8.gfx[64], 0);
        EXPECT_EQ(myChip8.gfx[65], 0);
        EXPECT_EQ(myChip8.V[0xF], 1);  // Collision occurred
    }

    void OpcodeEX9ETest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.key[0x5] = 1;

        myChip8.memory[0x202] = 0xE3;
        myChip8.memory[0x203] = 0x9E;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x05);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void OpcodeEX9ENoSkipTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xE3;
        myChip8.memory[0x203] = 0x9E;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x05);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void OpcodeEXA1Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xE3;
        myChip8.memory[0x203] = 0xA1;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x05);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void OpcodeEXA1NoSkipTest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.key[0x5] = 1;

        myChip8.memory[0x202] = 0xE3;
        myChip8.memory[0x203] = 0xA1;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x05);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void OpcodeFX07Test() {
        myChip8.initialize();

        myChip8.delay_timer = 0x42;

        myChip8.memory[0x200] = 0xF3;
        myChip8.memory[0x201] = 0x07;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x42);
        EXPECT_EQ(myChip8.delay_timer, 0x42);
        EXPECT_EQ(myChip8.pc, 0x202);
    }

    void OpcodeFX0ATest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0xF3;
        myChip8.memory[0x201] = 0x0A;
        
        // First cycle - no key pressed, PC shouldn't advance
        myChip8.emulateCycle();
        EXPECT_EQ(myChip8.pc, 0x200);

        myChip8.key[0x5] = 1;
        
        myChip8.emulateCycle();
        
        EXPECT_EQ(myChip8.V[0x3], 0x5);
        EXPECT_EQ(myChip8.pc, 0x202);
    }

    void OpcodeFX15Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xF3;
        myChip8.memory[0x203] = 0x15;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x42);
        EXPECT_EQ(myChip8.delay_timer, 0x42);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void OpcodeFX18Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xF3;
        myChip8.memory[0x203] = 0x18;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x42);
        EXPECT_EQ(myChip8.sound_timer, 0x42);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void OpcodeFX1ETest() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0xA2;
        myChip8.memory[0x201] = 0x34;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x63;
        myChip8.memory[0x203] = 0x42;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0xF3;
        myChip8.memory[0x205] = 0x1E;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x42);
        EXPECT_EQ(myChip8.I, 0x276);  // 0x234 + 0x42 = 0x276
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void OpcodeFX29Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x05;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xF3;
        myChip8.memory[0x203] = 0x29;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 0x05);
        EXPECT_EQ(myChip8.I, 0x05 * 5);
        EXPECT_EQ(myChip8.pc, 0x204);

        EXPECT_EQ(myChip8.memory[myChip8.I], 0xF0);
        EXPECT_EQ(myChip8.memory[myChip8.I + 1], 0x80);
        EXPECT_EQ(myChip8.memory[myChip8.I + 2], 0xF0);
        EXPECT_EQ(myChip8.memory[myChip8.I + 3], 0x10);
        EXPECT_EQ(myChip8.memory[myChip8.I + 4], 0xF0);
    }

    void OpcodeFX33Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x63;
        myChip8.memory[0x201] = 0x9C;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xA4;
        myChip8.memory[0x203] = 0x00;
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0xF3;
        myChip8.memory[0x205] = 0x33;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0x3], 156);
        EXPECT_EQ(myChip8.memory[0x400], 1);
        EXPECT_EQ(myChip8.memory[0x401], 5);
        EXPECT_EQ(myChip8.memory[0x402], 6);
        EXPECT_EQ(myChip8.pc, 0x206);
    }

    void OpcodeFX55Test() {
        myChip8.initialize();

        myChip8.memory[0x200] = 0x60;
        myChip8.memory[0x201] = 0x11;  // V0 = 0x11
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0x61;
        myChip8.memory[0x203] = 0x22;  // V1 = 0x22
        myChip8.emulateCycle();

        myChip8.memory[0x204] = 0x62;
        myChip8.memory[0x205] = 0x33;  // V2 = 0x33
        myChip8.emulateCycle();

        myChip8.memory[0x206] = 0x63;
        myChip8.memory[0x207] = 0x44;  // V3 = 0x44
        myChip8.emulateCycle();

        myChip8.memory[0x208] = 0xA4;
        myChip8.memory[0x209] = 0x00;  // I = 0x400
        myChip8.emulateCycle();

        myChip8.memory[0x20A] = 0xF3;
        myChip8.memory[0x20B] = 0x55;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.memory[0x400], 0x11);
        EXPECT_EQ(myChip8.memory[0x401], 0x22);
        EXPECT_EQ(myChip8.memory[0x402], 0x33);
        EXPECT_EQ(myChip8.memory[0x403], 0x44);
        EXPECT_EQ(myChip8.pc, 0x20C);
    }

    void OpcodeFX65Test() {
        myChip8.initialize();

        myChip8.memory[0x400] = 0x11;
        myChip8.memory[0x401] = 0x22;
        myChip8.memory[0x402] = 0x33;
        myChip8.memory[0x403] = 0x44;

        myChip8.memory[0x200] = 0xA4;
        myChip8.memory[0x201] = 0x00;
        myChip8.emulateCycle();

        myChip8.memory[0x202] = 0xF3;
        myChip8.memory[0x203] = 0x65;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.V[0], 0x11);
        EXPECT_EQ(myChip8.V[1], 0x22);
        EXPECT_EQ(myChip8.V[2], 0x33);
        EXPECT_EQ(myChip8.V[3], 0x44);
        EXPECT_EQ(myChip8.pc, 0x204);
    }

    void Opcode00EETest() {
        myChip8.initialize();
        
        myChip8.stack[0] = 0x300;
        myChip8.sp = 1;

        myChip8.memory[0x200] = 0x00;
        myChip8.memory[0x201] = 0xEE;
        myChip8.emulateCycle();

        EXPECT_EQ(myChip8.sp, 0);
        EXPECT_EQ(myChip8.pc, 0x302);
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

TEST_F(Chip8Test, Opcode6XNN) {
    Opcode6XNNTest();
}

TEST_F(Chip8Test, Opcode7XNN) {
    Opcode7XNNTest();
}

TEST_F(Chip8Test, Opcode8XY0) {
    Opcode8XY0Test();
}

TEST_F(Chip8Test, Opcode8XY1) {
    Opcode8XY1Test();
}

TEST_F(Chip8Test, Opcode8XY2) {
    Opcode8XY2Test();
}

TEST_F(Chip8Test, Opcode8XY3) {
    Opcode8XY3Test();
}

TEST_F(Chip8Test, Opcode8XY4) {
    Opcode8XY4Test();
}

TEST_F(Chip8Test, Opcode8XY4NoCarry) {
    Opcode8XY4NoCarryTest();
}

TEST_F(Chip8Test, Opcode8XY5) {
    Opcode8XY5Test();
}

TEST_F(Chip8Test, Opcode8XY5Borrow) {
    Opcode8XY5BorrowTest();
}

TEST_F(Chip8Test, Opcode8XY6) {
    Opcode8XY6Test();
}

TEST_F(Chip8Test, Opcode8XY6NoFlag) {
    Opcode8XY6NoFlagTest();
}

TEST_F(Chip8Test, Opcode8XY7) {
    Opcode8XY7Test();
}

TEST_F(Chip8Test, Opcode8XY7Borrow) {
    Opcode8XY7BorrowTest();
}

TEST_F(Chip8Test, Opcode8XYE) {
    Opcode8XYETest();
}

TEST_F(Chip8Test, Opcode8XYENoFlag) {
    Opcode8XYENoFlagTest();
}

TEST_F(Chip8Test, Opcode9XY0) {
    Opcode9XY0Test();
}

TEST_F(Chip8Test, Opcode9XY0NoSkip) {
    Opcode9XY0NoSkipTest();
}

TEST_F(Chip8Test, OpcodeANNN) {
    OpcodeANNNTest();
}

TEST_F(Chip8Test, OpcodeBNNN) {
    OpcodeBNNNTest();
}

TEST_F(Chip8Test, OpcodeCXNN) {
    OpcodeCXNNTest();
}

TEST_F(Chip8Test, OpcodeDXYN) {
    OpcodeDXYNTest();
}

TEST_F(Chip8Test, OpcodeEX9E) {
    OpcodeEX9ETest();
}

TEST_F(Chip8Test, OpcodeEX9ENoSkip) {
    OpcodeEX9ENoSkipTest();
}

TEST_F(Chip8Test, OpcodeEXA1) {
    OpcodeEXA1Test();
}

TEST_F(Chip8Test, OpcodeEXA1NoSkip) {
    OpcodeEXA1NoSkipTest();
}

TEST_F(Chip8Test, OpcodeFX07) {
    OpcodeFX07Test();
}

TEST_F(Chip8Test, OpcodeFX0A) {
    OpcodeFX0ATest();
}

TEST_F(Chip8Test, OpcodeFX15) {
    OpcodeFX15Test();
}

TEST_F(Chip8Test, OpcodeFX18) {
    OpcodeFX18Test();
}

TEST_F(Chip8Test, OpcodeFX1E) {
    OpcodeFX1ETest();
}

TEST_F(Chip8Test, OpcodeFX29) {
    OpcodeFX29Test();
}

TEST_F(Chip8Test, OpcodeFX33) {
    OpcodeFX33Test();
}

TEST_F(Chip8Test, OpcodeFX55) {
    OpcodeFX55Test();
}

TEST_F(Chip8Test, OpcodeFX65) {
    OpcodeFX65Test();
}

TEST_F(Chip8Test, Opcode00EE) {
    Opcode00EETest();
}
