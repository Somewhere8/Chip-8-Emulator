#pragma once
#include <stdint.h>
#include <stdlib.h>
#define STACK_SIZE     16
#define NUM_KEYS       16
#define DISPLAY_W      64
#define DISPLAY_H      32
#define PCINC           2
#define KEYPRESS        1
typedef enum {
    /*
    nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
    n or nibble - A 4-bit value, the lowest 4 bits of the instruction
    x - A 4-bit value, the lower 4 bits of the high byte of the instruction
    y - A 4-bit value, the upper 4 bits of the low byte of the instruction
    kk or byte - An 8-bit value, the lowest 8 bits of the instruction
    */
    CLS       = 0x00E0,  // Clear the screen
    RET       = 0x00EE,  // Return from subroutine
    
    JUMP      = 0x1000,  // Jump to address NNN
    CALL      = 0x2000,  // Call subroutine at NNN
    SE        = 0x3000,  // Skip if VX == KK
    SNE       = 0x4000,  // Skip if VX != KK
    SE_VX_VY  = 0x5000,  // Skip if VX == VY
    LD_VX_KK  = 0x6000,  // VX = KK
    ADD_VX_KK = 0x7000,
   
    LD_VX_VY  = 0x8000,  // VX = VY
    OR        = 0x8001,  // VX |= VY
    AND       = 0x8002,  // VX &= VY
    XOR       = 0x8003,  // VX ^= VY
    ADD_VX_VY = 0x8004,  // VX += VY, VF = carry
    SUB       = 0x8005,  // VX -= VY, VF = no borrow
    SHR       = 0x8006,  // VX >>= 1, VF = shifted bit
    SUBN      = 0x8007,  // VX = VY - VX, VF = no borrow
    SHL       = 0x800E,  // VX <<= 1, VF = shifted bit
    
    SNE_VX_VY = 0x9000,  // Skip if VX != VY
      
    LD_I      = 0xA000,  // I = NNN
    JUMP_V0   = 0xB000,  // Jump to NNN + V0
    RND       = 0xC000,  // VX = rand() & KK
    DRW       = 0xD000,  // draw sprite at (VX, VY), VF = collision
    
    SKP       = 0xE09E,  // skip if key VX pressed
    SKNP      = 0xE0A1,  // skip if key VX not pressed
    
    LD_VX_DT  = 0xF007,  // VX = delay timer
    LD_VX_K   = 0xF00A,  // VX = next keypress (blocking)
    LD_DT_VX  = 0xF015,  // delay timer = VX
    LD_ST_VX  = 0xF018,  // sound timer = VX
    ADD_I_VX  = 0xF01E,  // I += VX
    LD_F_VX   = 0xF029,  // I = sprite address for digit VX
    LD_B_VX   = 0xF033,  // Store BCD of VX at I, I+1, I+2
    LD_I_VX   = 0xF055,  // Store V0-VX in memory at I
    LD_VX_I   = 0xF065,  // Read V0-VX from memory at I

} opcodes;

typedef struct {
    // General purpose registers
    uint8_t V[16]; // V0-VF
    
    // Special Registers
    uint16_t I;   // address register
    uint16_t PC;  // program counter
    uint8_t SP;   // stack pointer
    
    uint8_t DT;   // delay timer
    uint8_t ST;   // sound timer

} Register;

extern Register reg;

extern void chip8(uint16_t input);