#include "cpu.h"
#include "memory.h"
#include "display.h"
#include "keys.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#define SCALE   10
int main(int argc, char* argv[]) {
    if (argc < 2) {
        SDL_Log("Usage: %s <rom_path>", argv[0]);
        return -1;
    }
    FILE* rom = fopen(argv[1], "rb");
    if(rom == NULL){
        fprintf(stderr, "Failed to open file (fnf)");
        return -1;
    }
    
    fseek(rom, 0, SEEK_END);
    long romSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);
    uint8_t* romBuffer = malloc(romSize);
    fread(romBuffer, 1, romSize, rom);
    loadROM(romBuffer, romSize);
    fclose(rom);
    free(romBuffer);
    loadFont(fontSet, sizeof(fontSet));
    reg.PC = 0x200;
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window* window = SDL_CreateWindow("Chip-8", HORIZONTAL_SIZE * SCALE, VERTICAL_SIZE * SCALE, 
        SDL_WINDOW_OPENGL
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL); 
    SDL_RenderClear(renderer);

    bool isRunning = true;
    SDL_Event event;
    //main loop
    while (isRunning) {
        uint16_t opcode = (mem[reg.PC] << 8) | mem[reg.PC + 1];
        //printf("PC: %04X  Opcode: %04X  DT: %d\n", reg.PC, opcode, reg.DT);
        chip8(opcode);
        

        if (reg.DT > 0) reg.DT--;
        if (reg.ST > 0) reg.ST--;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int y = 0; y < VERTICAL_SIZE; y++) {
            for (int x = 0; x < HORIZONTAL_SIZE; x++) {
                if (display[y * HORIZONTAL_SIZE + x]) {
                    SDL_FRect rect = { x * SCALE, y * SCALE, SCALE, SCALE };
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_1: keys[0x1] = 1; break;
                    case SDLK_2: keys[0x2] = 1; break;
                    case SDLK_3: keys[0x3] = 1; break;
                    case SDLK_4: keys[0xC] = 1; break;
                    case SDLK_Q: keys[0x4] = 1; break;
                    case SDLK_W: keys[0x5] = 1; break;
                    case SDLK_E: keys[0x6] = 1; break;
                    case SDLK_R: keys[0xD] = 1; break;
                    case SDLK_A: keys[0x7] = 1; break;
                    case SDLK_S: keys[0x8] = 1; break;
                    case SDLK_D: keys[0x9] = 1; break;
                    case SDLK_F: keys[0xE] = 1; break;
                    case SDLK_Z: keys[0xA] = 1; break;
                    case SDLK_X: keys[0x0] = 1; break;
                    case SDLK_C: keys[0xB] = 1; break;
                    case SDLK_V: keys[0xF] = 1; break;
                }
            }
            else if (event.type == SDL_EVENT_KEY_UP) {
                switch (event.key.key) {
                    case SDLK_1: keys[0x1] = 0; break;
                    case SDLK_2: keys[0x2] = 0; break;
                    case SDLK_3: keys[0x3] = 0; break;
                    case SDLK_4: keys[0xC] = 0; break;
                    case SDLK_Q: keys[0x4] = 0; break;
                    case SDLK_W: keys[0x5] = 0; break;
                    case SDLK_E: keys[0x6] = 0; break;
                    case SDLK_R: keys[0xD] = 0; break;
                    case SDLK_A: keys[0x7] = 0; break;
                    case SDLK_S: keys[0x8] = 0; break;
                    case SDLK_D: keys[0x9] = 0; break;
                    case SDLK_F: keys[0xE] = 0; break;
                    case SDLK_Z: keys[0xA] = 0; break;
                    case SDLK_X: keys[0x0] = 0; break;
                    case SDLK_C: keys[0xB] = 0; break;
                    case SDLK_V: keys[0xF] = 0; break;
                }
            }
        }
        SDL_Delay(2);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
};
