#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include "chip8.h"

chip8* emu;

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

int display_width = SCREEN_WIDTH * 10;
int display_height = 10 * SCREEN_HEIGHT;

void display();

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


void setKeys(chip8 * param){
    return;
}

void drawPixel(int x, int y){
	glBegin(GL_QUADS);
		glVertex3f((x * 10) + 0.0f,     (y * 10) + 0.0f,	 0.0f);
		glVertex3f((x * 10) + 0.0f,     (y * 10) + 10, 0.0f);
		glVertex3f((x * 10) + 10, (y * 10) + 10, 0.0f);
		glVertex3f((x * 10) + 10, (y * 10) + 0.0f,	 0.0f);
	glEnd();
}


void updateQuadrants(chip8 * emu){
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
		{
			if(emu->gfx[(y*64) + x] == 0) 
				glColor3f(0.0f,0.0f,0.0f);			
			else 
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
}

void display(){
	emulateCycle(emu);
		
	if(emu->drawflag)
	{
		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
        
		updateQuadrants(emu);		

		// Swap buffers!
		glutSwapBuffers();    

		// Processed frame
		emu->drawflag = 0;
	}
}

void reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	display_width = w;
	display_height = h;
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
    glutReshapeFunc(reshape_window);
    printf("%d", loadApplication("../roms/ibm.ch8", emu));

    glutMainLoop();
    return 0;
}

