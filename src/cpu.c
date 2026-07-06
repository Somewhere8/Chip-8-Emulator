#include "cpu.h"
#include "memory.h"
#include "display.h"
#include "keys.h"
Register reg = {0};

void execute(uint16_t input) {
    uint16_t checkcase = input & 0xF000;
    uint16_t nnn = input & 0x0FFF;
    uint8_t n = input & 0x000F;
    uint8_t x = (input >> 8) & 0x000F;
    uint8_t y = (input >> 4) & 0x000F;
    uint8_t kk = input & 0x00FF;
    reg.PC += PCINC;
    switch (checkcase) {
        case 0x0000:
            switch (input) {
                case(CLS):
                    for(int i = 0; i < VERTICAL_SIZE * HORIZONTAL_SIZE; i++)
                        display[i] = 0;
                    break;
                case(RET):
                    reg.SP--;
                    reg.PC = stack[reg.SP];
                    break;
                default : // SYS
                    break;
            }
            break;
        case (JUMP):
            reg.PC = nnn;
            break;
        case (CALL):
            stack[reg.SP] = reg.PC;
            reg.PC = nnn;
            reg.SP++;
            break;
        case (SE): //skip
            if(reg.V[x] == kk)
                reg.PC += PCINC;
            break;
        case (SNE):
            if(reg.V[x] != kk)
                reg.PC += PCINC;
            break;
        case (SE_VX_VY):
            if(reg.V[x] == reg.V[y])
                reg.PC += PCINC;
            break;
        case (LD_VX_KK):
            reg.V[x] = kk;
            break;
        case (ADD_VX_KK):
            int sum = reg.V[x] + kk;
            reg.V[x] = sum & 0xFF;
            break;
        case 0x8000:
            checkcase = input & 0xF00F;
            switch (checkcase) {
                case (LD_VX_VY):
                    reg.V[x] = reg.V[y];   
                    break;
                case (OR):
                    reg.V[x] |= reg.V[y];
                    break;
                case (AND):
                    reg.V[x] &= reg.V[y];
                    break;
                case (XOR):
                    reg.V[x] ^= reg.V[y];
                    break;
                case (ADD_VX_VY):
                    int sum = reg.V[x] + reg.V[y];
                    reg.V[x] = sum & 0xFF;
                    reg.V[15] = sum > 0xFF ? 1 : 0;
                    break;
                case (SUB):
                    reg.V[x] -= reg.V[y];
                    reg.V[15] = reg.V[x] >= reg.V[y] ? 1 : 0;
                    break;
                case (SHR): //Config the shift??? vy = vx or vx in place
                    //reg.V[x] = reg.V[y];
                    reg.V[x] >>= 1;
                    reg.V[15] = reg.V[x] & 0x1;
                    break;
                case (SUBN):
                    reg.V[x] = reg.V[y] - reg.V[x];
                    reg.V[15] = reg.V[y] >= reg.V[x] ? 1 : 0;
                    
                    break;
                case (SHL): //Config the shift??? vy = vx or vx in place
                    //reg.V[x] = reg.V[y];
                    reg.V[x] <<= 1;
                    reg.V[15] = (reg.V[x] >> 7) & 0x1;
                    break;
            }
            break;
        case (SNE_VX_VY):
            if(reg.V[x] != reg.V[y])
                reg.PC += PCINC;
            break;
        case (LD_I):
            reg.I = nnn;
            break;
        case (JUMP_V0):
            reg.PC = nnn + reg.V[0];
            break;
        case (RND):
            reg.V[x] = (rand() % 0x100) & kk; 
            break;
        case (DRW):
            uint8_t xpos = reg.V[x] % HORIZONTAL_SIZE;
            uint8_t ypos = reg.V[y] % VERTICAL_SIZE;
            reg.V[15] = 0;
            for(uint8_t i = 0; i < n; i++) {
                if(ypos + i >= VERTICAL_SIZE)
                    break;
                uint8_t sprite = mem[reg.I + i];
                for(int j = 0; j < BYTE; j++) {
                    if(xpos + j >= HORIZONTAL_SIZE)
                        break;
                    uint8_t checkbits = sprite & (1 << (BYTE - 1 - j));
                    if(checkbits){
                        if(display[(ypos + i) * HORIZONTAL_SIZE + (xpos + j)] == 1)
                            reg.V[15] = 1;
                        display[(ypos + i) * HORIZONTAL_SIZE + (xpos + j)] ^= 1;
                    }
                }
            }

            break;
        case 0xE000:
            checkcase = input & 0xF0FF;
            switch (checkcase) {
                case (SKP):
                    if(keys[reg.V[x]] == KEYPRESS)
                        reg.PC += PCINC;
                    break;
                case (SKNP):
                    if(keys[reg.V[x]] != KEYPRESS)
                        reg.PC += PCINC;
                    break;
            }
            break;
        case 0xF000:
            checkcase = input & 0xF0FF;
            switch (checkcase) {
                case (LD_VX_DT):
                    reg.V[x] = reg.DT;
                    break;
                case (LD_VX_K):
                    uint8_t isPressed = 0;
                    for(int i = 0; i <= 0xF; i++) {
                        if(keys[i] == 1) {
                            reg.V[x] = i;
                            isPressed = KEYPRESS;
                            break;
                        }
                    }
                    if(!isPressed){
                        reg.PC -= PCINC;
                    }
                    break;
                case (LD_DT_VX):
                    reg.DT = reg.V[x];
                    break;
                case (LD_ST_VX):
                    reg.ST = reg.V[x];
                    break;
                case (ADD_I_VX):
                    reg.I += reg.V[x];
                    break;
                case (LD_F_VX):
                    reg.I = reg.V[x] * 5;
                    break;
                case (LD_B_VX):
                    mem[reg.I] = reg.V[x] / 100;
                    mem[reg.I + 1] = (reg.V[x] / 10) % 10;
                    mem[reg.I + 2] = reg.V[x] % 10;
                    break;
                case (LD_I_VX):
                    for(uint8_t i = 0; i <= x; i++)
                        mem[reg.I + i] = reg.V[i]; //config if I increments
                    break;
                case (LD_VX_I):
                    for(uint8_t i = 0; i <= x; i++)
                        reg.V[i] = mem[reg.I + i]; //config if I increments
                    break;
            }
            break;
    }
};