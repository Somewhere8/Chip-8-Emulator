#pragma once
#include <stdint.h>
#define MEMORY_SIZE 4096
#define STACK_SIZE 16
extern uint16_t mem[MEMORY_SIZE];
extern uint16_t stack[STACK_SIZE];
extern const uint8_t fontSet[80];
extern void loadFont(uint8_t *font, size_t size);
extern void loadROM(uint8_t *rom, size_t size);