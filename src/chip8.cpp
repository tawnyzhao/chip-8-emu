#include "chip8.hpp"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define REG_SIZE 16
#define MEM_SIZE 4096
#define ROM_START 512
#define GFX_SIZE 64*32
#define STACK_SIZE 16


const unsigned char chip8_fontset[80] = {
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

chip8::chip8() {
    // Empty
}

chip8::~chip8() {
    // Empty
}

void chip8::loadROM(char *filename) {
    init(); 
    FILE *rom = fopen(filename, "rb");
    int i = ROM_START;
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
    pc = ROM_START;
    opcode = 0;
    index = 0;
    sp = 0;

    for (int i = 0; i < GFX_SIZE; i++) {
        gfx[i] = 0;
    } 
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = 0;
        key[i] = 0;
        V[i] = 0;
    }
    for (int i = 0; i < MEM_SIZE; i++) {
        memory[i]++;
    }
    
    for (int i = 0; i < 80; i++) { //Load fonts
        memory[i] = chip8_fontset[i];
    }

    delay_timer = 0;
    sound_timer = 0;
}

void chip8::runOpcode() {
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
        case 0x1000:  // Jumps to NNN
            pc = opcode & 0x0FFF;
            break;

        case 0x2000:  // Calls subroutine at NNN
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;

        case 0x3000:
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

        case 0x4000:
            if ((V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

        case 0x5000:
            if ((V[opcode & 0x0F00 >> 8]) == (V[opcode & 0x00F0 >> 4])) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

        case 0x6000:
            V[opcode & 0x0F00 >> 8] = opcode & 0x00FF;
            pc += 2;
            break;

        case 0x7000:
            V[opcode & 0x0F00 >> 8] += opcode & 0x00FF;
            pc += 2;
            break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    V[opcode & 0x0F00 >> 8] = V[opcode & 0x00F0 >> 4];
                    pc += 2;
                    break;
                case 0x0001:
                    V[opcode & 0x0F00 >> 8] |= V[opcode & 0x00F0 >> 4];
                    pc += 2;
                    break;
                case 0x0002:
                    V[opcode & 0x0F00 >> 8] &= V[opcode & 0x00F0 >> 4];
                    pc += 2;
                    break;
                case 0x0003:
                    V[opcode & 0x0F00 >> 8] ^= V[opcode & 0x00F0 >> 4];
                    pc += 2;
                    break;
                case 0x0004:
                    if (V[opcode & 0x0F00 >> 8] >
                        (0xFF - V[opcode & 0x00F0 >> 4])) {
                        V[0x000F] = 1;
                    } else {
                        V[0x000F] = 0;
                    }
                    V[opcode & 0x0F00 >> 8] ^= V[opcode & 0x00F0 >> 4];
                    pc += 2;
                    break;

                case 0x0005:
                    if (V[opcode & 0x0F00 >> 8] < V[opcode & 0x00F0 >> 4]) {
                        V[0x000F] = 0;
                    } else {
                        V[0x000F] = 1;
                    }
                    V[opcode & 0x0F00 >> 8] -= V[opcode & 0x00F0 >> 4];
                    pc += 2;
                    break;

                case 0x0006:  // Bitshift Right
                    V[0x000F] = V[opcode & 0x0F00 >> 8] & 0x0001;
                    V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] >> 1;
                    pc += 2;
                    break;

                case 0x0007:
                    if (V[opcode & 0x0F00 >> 4] < V[opcode & 0x00F0 >> 8]) {
                        V[0x000F] = 0;
                    } else {
                        V[0x000F] = 1;
                    }
                    V[opcode & 0x0F00 >> 8] =
                        V[opcode & 0x00F0 >> 4] - V[opcode & 0x0F00 >> 8];
                    pc += 2;
                    break;

                case 0x000E:
                    V[0x000F] = V[opcode & 0x8000 >> 8] >> 7;
                    V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] << 1;
                    pc += 2;
                    break;
            }
            break;
        case 0x9000:
            if (V[opcode & 0x0F00 >> 8] != V[opcode & 0x00F0 >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

        case 0xA000:
            index = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xB000:
            pc = opcode & 0x0FFF + V[0x0000];
            break;

        case 0xC000:  // sets VX to randint
            V[opcode & 0x0F00 >> 8] =
                opcode & 0x0FF;  // TODO: Actually set to randint
            pc += 2;
            break;

        case 0xD000:
            unsigned short pixel;
            V[0x000F] = 0;

            for (int i = 0; i < opcode & 0x000F; i++) {
                pixel = memory[index + i];
                for (int j = 0; j < 8; j++) {
                    if (pixel & (0x80 >> j) != 0) {
                        if (gfx[V[opcode & 0x0F00 >> 8] + j +
                                (V[opcode & 0x00F0 >> 4] + i) * 64] == 1) {
                            V[0xF] = 1;
                        } else {
                            gfx[V[opcode & 0x0F00 >> 8] + j +
                                (V[opcode & 0x00F0 >> 4] + i) * 64] = 1;
                        }
                    }
                }
            }
            pc += 2;
            break;
        case 0xE000:
            switch (opcode & 0x000F) {
                case 0x0001:
                    if (key[opcode & 0x0F000 >> 8] !=
                        V[opcode & 0x0F000 >> 8]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                case 0x000E:
                    if (key[opcode & 0x0F000 >> 8] ==
                        V[opcode & 0x0F000 >> 8]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x0FF) {
                case 0x0007:
                    V[opcode & 0x0F000 >> 8] = delay_timer;
                    pc += 2;
                    break;
                case 0x000A: {
                    bool key_pressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (key[i]) {
                            V[opcode & 0x0F000 >> 8] = key[i];
                            key_pressed = true;
                        }
                    }
                    if (key_pressed) {
                        pc += 2;
                    }
                    break;
                }
                case 0x0015:
                    delay_timer = V[opcode & 0x0F000 >> 8];
                    pc += 2;
                    break;
                case 0x0018:
                    sound_timer = V[opcode & 0x0F000 >> 8];
                    pc += 2;
                    break;
                case 0x001E:
                    if (index > 0xFFF0 - V[opcode & 0x0F000 >> 8]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    index += V[opcode & 0x0F000 >> 8];
                    pc += 2;
                    break;
                case 0x0029:
                    index = V[opcode & 0x0F000 >> 8] * sizeof(unsigned) * 5;
                    pc += 2;
                    break;

                case 0x0033:
                    memory[index] = V[opcode & 0x0F000 >> 8] /
                                    100;  // Most significant first
                    memory[index + 1] = V[opcode & 0x0F000 >> 8] / 10 % 10;
                    memory[index + 2] = V[opcode & 0x0F000 >> 8] % 100;
                    pc += 2;
                    break;
                case 0x0055:
                    for (int i = 0; i<opcode & 0x0F000>> 8; i++) {
                        memory[index + i] = V[i];
                    }
                    pc += 2;
                    break;

                case 0x0065:
                    for (int i = 0; i<opcode & 0x0F000>> 8; i++) {
                        V[i] = memory[index + 1];
                    }
                    pc += 2;
                    break;
            }
            break;
        default:
            printf("Unknown opcode: %X\n", opcode);
    }
}

void chip8::updateTimers() {
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

void chip8::emulateCycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];  // get 2 byte opcode from memory

    runOpcode();

    updateTimers();
}

void chip8::printScreen() {
    
}