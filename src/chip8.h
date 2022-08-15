#include <stdint.h>

typedef __uint8_t u8;
typedef __uint16_t u16;

#define MEM_SIZE 4096
#define NUM_REGISTERS 16
#define STACK_SIZE 16
#define KEY_LENGTH 16
#define FONTSET_SIZE 80

struct chip8{
    int drawflag;
    u8 gfx[64 * 32];
    u8 key[KEY_LENGTH];
    u16 pc;
    u16 opcode;
    u16 index_register;
    u16 stack_pointer;

    u8 registers[NUM_REGISTERS];
    u16 stack[STACK_SIZE];
    u8 memory[MEM_SIZE];

    u8 delay_timer;
    u8 sound_timer;

    
}
typedef chip8;

void setKeys(chip8 *);
void emulateCycle(chip8 *);
void debugRender();
int loadApplication(const char * filename, chip8 *);
void init(chip8 *);
void clear_display();