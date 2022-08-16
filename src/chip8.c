#include "chip8.h"
#include <stdio.h>

const unsigned char fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};

void emulateCycle(chip8* emu){
    u16 opcode = emu->memory[emu->pc] << 8 | emu->memory[(emu->pc)+1];
    // debug out printf("%04x\n", opcode);
    switch (opcode & 0xF000){
        
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x000E:
                    emu->stack_pointer -= 1;
                    emu->pc += 2;
                    break;
                case 0x0000:
                    for(int i = 0; i < 2048; ++i){
                        emu->gfx[i] = 0x0;
                    }
                    emu->drawflag = 1;
                    emu->pc += 2;
                    break;
            }
            break;
        case 0x1000:
            emu->pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            emu->stack[emu->stack_pointer] = emu->pc;
            emu->stack_pointer += 1;
            emu->pc = opcode & 0x0FFF;
            break;
        case 0x3000:
            if (emu->registers[(opcode & 0x0F00) >> 8] == opcode & 0x00FF)
                emu->pc += 4;
            else 
                emu->pc +=2;
            break;
        case 0x4000:
            if (emu->registers[(opcode & 0x0F00) >> 8] != opcode & 0x00FF)
                emu->pc += 4;
            else
                emu->pc +=2;
            break;
        case 0x5000:
            if (emu->registers[(opcode & 0x0F00) >> 8] == emu->registers[(opcode & 0x00F0) >> 4])
                emu->pc += 4;
            else
                emu->pc += 2;
            break;
        case 0x6000:
            emu->registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            emu->pc += 2;
            break;
        case 0x7000:
            emu->registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            emu->pc += 2;
            break;
        case 0xA000:
            emu->index_register = opcode & 0x0FFF;
            emu->pc += 2;
            break;
        case 0xD000:
            {
                u16 x = emu->registers[(opcode & 0x0F00) >> 8];
                u16 y = emu->registers[(opcode & 0x00F0) >> 4];
                u16 height = opcode & 0x000F;
                u16 p;
                emu->registers[15] = 0;
                for (int yl = 0; yl < height; ++yl) // yl increases up to height {
                    {
                    p = emu->memory[emu->index_register + yl];
                    for (int xl = 0; xl < 8; ++xl){ // xl is fixed at 8
                        if ((p & (0x80 >> xl)) != 0){
                            if (emu->gfx[(x+xl+((y+yl)*64))] == 1){
                                emu->registers[15] = 1; // set the F register to indicate that screen pixels were flipped from set to unset
                            }
                            emu->gfx[x + xl + ((y+yl)*64)] ^= 1;
                        }
                    }
                }
                emu->drawflag = 1;
                emu->pc += 2;
            }
            break;

        default:
            printf("Error, opcode: %04x not implemented yet.\n", opcode);
    }
    if (emu->delay_timer > 0) --emu->delay_timer;
    if (emu->sound_timer > 0) --emu->sound_timer;

}

void init(chip8* emu){
    
    emu->pc = 0x200;
    emu->opcode = 0;
    emu->index_register = 0;
    emu->stack_pointer = 0;

    for (int i = 0; i < MEM_SIZE; ++i){
        emu->memory[i] = 0;
    }

    for (int i = 0; i < NUM_REGISTERS; ++i){
        emu->registers[i] = 0;
    }

    for (int i = 0 ; i < STACK_SIZE; ++i){
        emu->stack[i] = 0;
    }

    for (int i = 0; i < FONTSET_SIZE; ++i){
        emu->memory[i] = fontset[i];
    }

    emu->sound_timer = 0;
    emu->delay_timer = 0;

}