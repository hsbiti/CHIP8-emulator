//
// Created by Hamza Sbiti on 18/05/2016.
//

#include "Chip8.h"
#include <stdio.h>
#include <stdlib.h>




unsigned char chip8_fontset[80] =
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0, //0
                0x20, 0x60, 0x20, 0x20, 0x70, //1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
                0x90, 0x90, 0xF0, 0x10, 0x10, //4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
                0xF0, 0x10, 0x20, 0x40, 0x40, //7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
                0xF0, 0x90, 0xF0, 0x90, 0x90, //A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
                0xF0, 0x80, 0x80, 0x80, 0xF0, //C
                0xE0, 0x90, 0x90, 0x90, 0xE0, //D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
                0xF0, 0x80, 0xF0, 0x80, 0x80  //F
        };




Chip8::Chip8()
{

}

void Chip8::initialize()
{
    pc     = 0x200;  // Program counter starts at 0x200
    opcode = 0;      // Reset current opcode
    I      = 0;      // Reset index register
    sp     = 0;      // Reset stack pointer

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;



    // Clear display
    for(int i = 0; i < 2048; ++i)
        gfx[i] = 0;

    drawFlag = true;

    // Clear stack
    for(int i = 0; i < 16; ++i)
        stack[i] = 0;

    for(int i = 0; i < 16; ++i)
        key[i] = V[i] = 0;

    // Clear memory
    for(int i = 0; i < 4096; ++i)
        memory[i] = 0;

    // Load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];
}



void Chip8::emulateCycle()
{

    opcode = memory[pc] << 8 | memory[pc + 1];

    // & pour ne récupérer que les 4 premiers bits
    switch(opcode & 0xF000)
    {

        case 0x0000: // Calls RCA 1802 program at address NNN. Not necessary for most ROMs.

            break;

        case 0x1000: // Jump à l'adresse NNN de 0xNNN

            break;

        case 0x2000: // Appel subroutine à NNN.

            break;

        case 0x3000: // Passe la prochaine instruction si VX == NN.

            break;

        case 0x4000: // Passe la prochaine instruction si VX != NN.

            break;

        case 0x5000: // 0x5XY0: Passe la prochaine instruction si if VX == VY.

            break;

        case 0x6000: // Change la valeur de VX en NN.

            break;

        case 0x7000: // 0x7XNN: Ajoute NN à VX.

            break;

        case 0x8000: // Grand switch qui dépendra de la valeur des NNN

            break;

        case 0x9000: // 0x9XY0: Passe la prochaine instruction si VX != VY

            break;

        case 0xA000: // Insère l'adresse NNN dans I

            break;

        case 0xB000: // Jumps à l'adresse NNN plus V0

            break;

        case 0xC000: // CXNN: Valeur aléatoire dans VX & NN

            break;

        case 0xD000: // Pour le dessin

            break;

        case 0xE000: // Switch qui traitera selon si la touche est pressée ou pas

            break;

        case 0xF000: // Timers et son

            break;

        default:
            printf ("Unknown opcode: 0x%X\n", opcode);
            break;

    }

}
