//
// Created by Hamza Sbiti on 18/05/2016.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include <iostram>

class Chip8
{

public:
    Chip8();

private:
    // Mémoire du programme
    unsigned char memory[4096];
    // Program Counter
    unsigned short PC = 0x200;
    // Tableau des registres, de 0x0 à 0xF
    unsigned char V[16];
    // Registre 16 bit
    unsigned short I;




};


#endif //CHIP8_EMULATOR_CHIP8_H
