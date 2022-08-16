#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

chip8* emu;

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

int display_width = SCREEN_WIDTH * 10;
int display_height = 10 * SCREEN_HEIGHT;

void display();
u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];
void setupTexture();

void keyboardUp(unsigned char a, int x, int y){
    return;
}

void keyboardDown(unsigned char a, int x, int y){
    return;
}

int initializeGraphics(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
    glutCreateWindow("ch8emu");
    return 0;
}

int initializeInput(){
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    return 0;
}

int drawGraphics(){
    // TODO
    return 1;
}

void setKeys(chip8 * param){
    return;
}


void updateQuadrants(){
    for (int y = 0; y < SCREEN_HEIGHT; ++y){
        for (int x = 0; x < SCREEN_WIDTH; ++x){
            if (emu->gfx[(y*SCREEN_WIDTH) + x] == 0) glColor3f(0.0f, 0.0f, 0.0f);
            else glColor3f(1.0f, 1.0f, 1.0f);
        }
    }
}

void display(){
    emulateCycle(emu);
    if (emu->drawflag){
        glClear(GL_COLOR_BUFFER_BIT);
        updateQuadrants();
    }

    glutSwapBuffers();
    emu->drawflag = 0;
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


int main(int argc, char ** argv){
    emu = (chip8 *) malloc(sizeof(chip8));
    initializeGraphics(argc, argv);
    initializeInput();
    init(emu);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    printf("%d", loadApplication("../roms/ibm.ch8", emu));

    glutMainLoop();
    // for(;;){
    //     emulateCycle(emu);
    //     if (emu->drawflag) drawGraphics();
    //     setKeys(emu);
    // }
    return 0;
}

