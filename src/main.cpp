//
// Created by Hamza Sbiti on 18/05/2016.
//

#include <iostream>
#include <unistd.h>
#include "Chip8.h"

using namespace std;

#define WIDTH 64
#define HEIGHT 32

int modifier = 10;
int display_width = WIDTH * modifier;
int display_height = HEIGHT * modifier;

Chip8 myChip8;

GLvoid drawPixel(int x, int y)
{
    glBegin(GL_QUADS);
    glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
    glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
    glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
    glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
    glEnd();
}


GLvoid updateQuads(const Chip8& c8)
{
    // Draw
    for(int y = 0; y < 32; ++y)
        for(int x = 0; x < 64; ++x)
        {
            if(c8.gfx[(y*64) + x] == 0)
                glColor3f(0.0f,0.0f,0.0f);
            else
                glColor3f(1.0f,1.0f,1.0f);

            drawPixel(x, y);
        }
}

GLvoid reshape_window(GLsizei w, GLsizei h)
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

void keyboardDown(unsigned char key, int x, int y)
{
    if(key == 27)    // esc
        exit(0);

    if(key == '1')		myChip8.key[0x1] = 1;
    else if(key == '2')	myChip8.key[0x2] = 1;
    else if(key == '3')	myChip8.key[0x3] = 1;
    else if(key == '4')	myChip8.key[0xC] = 1;

    else if(key == 'q')	myChip8.key[0x4] = 1;
    else if(key == 'w')	myChip8.key[0x5] = 1;
    else if(key == 'e')	myChip8.key[0x6] = 1;
    else if(key == 'r')	myChip8.key[0xD] = 1;

    else if(key == 'a')	myChip8.key[0x7] = 1;
    else if(key == 's')	myChip8.key[0x8] = 1;
    else if(key == 'd')	myChip8.key[0x9] = 1;
    else if(key == 'f')	myChip8.key[0xE] = 1;

    else if(key == 'z')	myChip8.key[0xA] = 1;
    else if(key == 'x')	myChip8.key[0x0] = 1;
    else if(key == 'c')	myChip8.key[0xB] = 1;
    else if(key == 'v')	myChip8.key[0xF] = 1;

    //printf("Press key %c\n", key);
}

void keyboardUp(unsigned char key, int x, int y)
{
    if(key == '1')		myChip8.key[0x1] = 0;
    else if(key == '2')	myChip8.key[0x2] = 0;
    else if(key == '3')	myChip8.key[0x3] = 0;
    else if(key == '4')	myChip8.key[0xC] = 0;

    else if(key == 'q')	myChip8.key[0x4] = 0;
    else if(key == 'w')	myChip8.key[0x5] = 0;
    else if(key == 'e')	myChip8.key[0x6] = 0;
    else if(key == 'r')	myChip8.key[0xD] = 0;

    else if(key == 'a')	myChip8.key[0x7] = 0;
    else if(key == 's')	myChip8.key[0x8] = 0;
    else if(key == 'd')	myChip8.key[0x9] = 0;
    else if(key == 'f')	myChip8.key[0xE] = 0;

    else if(key == 'z')	myChip8.key[0xA] = 0;
    else if(key == 'x')	myChip8.key[0x0] = 0;
    else if(key == 'c')	myChip8.key[0xB] = 0;
    else if(key == 'v')	myChip8.key[0xF] = 0;
}


GLvoid display()
{
    myChip8.emulateCycle();

    if(myChip8.drawFlag)
    {
        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);

        updateQuads(myChip8);

        // Swap buffers!
        glutSwapBuffers();

        // Processed frame
        myChip8.drawFlag = false;
    }
}



int main (int argc, char* argv[])
{

    if(argc < 2)
    {
        cerr << "Use: " << argv[0] << " rom" << endl;
        return 1;
    }


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize (display_width, display_height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[1]);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape_window);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);

    glutMainLoop();

    sleep(3);

    return 0;
}