#include "chip8.hpp"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define REG_SIZE 16
#define MEM_SIZE 4096
#define GFX_SIZE 64*32
#define STACK_SIZE 16

unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void chip8::loadROM(char *filename) {
    init();
    FILE *rom = fopen(filename, "rb");
    int i = 512;
    if (rom != NULL) {
        do {
            char c = fgetc(rom);
            memory[i++] = c;
        } while (feof(rom));
    }
    fclose(rom);
    return;
}
void chip8::init() {
    pc = 0x200;
    opcode = 0;
    index = 0;
    sp = 0;

    memset(gfx, 0, 64 * 32 * sizeof(unsigned char));  // Init gfx
    memset(stack, 0, 16 * sizeof(unsigned char));     // Init stack
    memset(V, 0, 16 * sizeof(unsigned short));        // Init register
    memset(memory, 0, 4096 * sizeof(unsigned char));  // Init memory

    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }

    delay_timer = 0;
    sound_timer = 0;
}

void chip8::emulateCycle() {
	opcode = memory[pc] << 8 | memory[pc + 1]; // get 2 byte opcode from memory

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000:  // 0x00E0: Clears the screen
                    for (int i = 0; i < GFX_SIZE; i++) {
                        gfx[i] = 0;
                    }
                    pc += 2;
                    break;
                case 0x000E:  // 0x00EE: Returns from subroutine
                    sp--;
                    pc = stack[sp];
                    pc += 2;
                    break;
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break; 
        case 0x1000: //Jumps to NNN
            pc = opcode & 0x0FFF;
            break;
            
        case 0x2000:
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;

        // TODO: Implement all opcodes
        default:
            printf("Unknown opcode: %X\n", opcode);
    }

    // TODO: emulate each cycle at 60Hz
    if (delay_timer > 0) {
        delay_timer--;
    }
    if (sound_timer > 0) {
        if (sound_timer == 1) {
            // TODO: Actually play a sound
            printf("Played a sound");
        }
        sound_timer--;
    }
}