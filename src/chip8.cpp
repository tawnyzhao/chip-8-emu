#include "chip8.hpp"
#include <stdio.h>
#include <string.h>

unsigned char chip8_fontset[80] = {0};
unsigned short getOpcode(unsigned char *memory) {
    return *memory << 8 | *(memory+1);
}

void chip8::init() {
    pc = 0x200;
    opcode = 0;
    index = 0;
    stack_pointer = 0;

    memset(gfx, 0, 64*32 * sizeof(unsigned char)); //Init gfx
    memset(stack, 0, 16 * sizeof(unsigned char)); //Init stack
    memset(V, 0, 16 * sizeof(unsigned short));//Init register
    memset(memory, 0, 4096 * sizeof(unsigned char));//Init memory

    for(int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }
    
    delay_timer = 0;
    sound_timer = 0;

    //TODO: Load the ROM into memory
}

void chip8::emulateCycle(){
    opcode = getOpcode(&memory[pc]);

    switch(opcode & 0xF000) {
        // TODO: Implement all opcodes
        default:
            printf("Unknown opcode: %X\n", opcode);
    }

    if (delay_timer > 0) {
        delay_timer--;
    }
    if (sound_timer > 0) {
        if (sound_timer == 1) {
            //TODO: Actually play a sound
            printf("Played a sound");
        }
        sound_timer--;
    }
}