//
// Created by Hamza Sbiti on 18/05/2016.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H


#ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
#endif

class Chip8
{

public:
    Chip8();
    void initialize();
    void emulateCycle();

private:

    unsigned short opcode;
    // Mémoire du programme
    unsigned char memory[4096];
    // Program Counter
    unsigned short PC;
    // Tableau des registres, de 0x0 à 0xF
    unsigned char V[16];
    // Registre 16 bit
    unsigned short I;

    // Graphics,  écran de 64 x 32 ==> 2048 pixels
    unsigned char gfx[64 * 32];

    unsigned char delay_timer;
    unsigned char sound_timer;

    // La pile
    unsigned short stack[16];
    // Le niveau courant de la pile (stack pointer)
    unsigned short sp;

    // Etat courant de la touche
    unsigned char key[16];

    bool drawFlag;


};


#endif //CHIP8_EMULATOR_CHIP8_H
