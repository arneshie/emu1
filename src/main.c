#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

chip8* emu;


int initializeGraphics(){
    // TODO
    return 1;
}

int initializeInput(){
    // TODO
    return 1;
}

int drawGraphics(){
    // TODO
    return 1;
}

void setKeys(chip8 * param){
    return;
}


int loadApplication(const char * filename, chip8* emu){
    FILE * f = fopen(filename, "rb");
    if (f == NULL){
        printf("File error\n");
        return -1;
    } 
    fseek(f, 0, SEEK_END);
    long lsize = ftell(f);
    rewind(f);
    char * buffer = (char*) malloc(sizeof(char) * lsize);
    size_t result = fread(buffer, 1, lsize, f);
    if ((4096-512) > lsize){
        for (int i = 0; i < lsize; ++i)
            emu->memory[i + 512] = buffer[i];
    }
    else{
        printf("Error, ROM too large.");
    }
    fclose(f);
    free(buffer);
    return 0;
}


int main(int arc, char ** argv){
    emu = (chip8 *) malloc(sizeof(chip8));
    initializeGraphics();
    initializeInput();
    init(emu);
    printf("%d\n", loadApplication("../roms/ibm.ch8", emu));
    for(;;){
        emulateCycle(emu);
        if (emu->drawflag) drawGraphics();
        setKeys(emu);
    }
    return 0;
}

