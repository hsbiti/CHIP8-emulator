//
// Created by Hamza Sbiti on 18/05/2016.
//

#include "Chip8.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


using namespace std;

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
    PC     = 0x200;  // Program counter starts at 0x200
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
    opcode = memory[PC] << 8 | memory[PC + 1];

    // & pour ne récupérer que les 4 premiers bits
    switch(opcode & 0xF000)
    {

        case 0x0000: // Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Vide l'écran
                    for(int i = 0; i < 2048; ++i)
                        gfx[i] = 0x0;
                    drawFlag = true;
                    PC += 2;
                    break;

                case 0x000E: // 0x00EE: Retour de subroutine
                    sp--;			// 16 niveaux du stack, décrémente sp pour éviter un overwrite
                    PC = stack[sp];	// Adresse enregistrée dans PC
                    PC += 2;
                    break;

                default:
                    cerr << "Unkown opcode [0x0000]:0x" << hex << opcode << endl;
            }
            break;

        case 0x1000: // Jump à l'adresse NNN de 0xNNN
            PC = opcode & 0x0FFF;
            break;

        case 0x2000: // Appel subroutine à NNN.
            stack[sp] = PC;			// Enregister l'adresse courante dans la pile
            sp++;					// incrémenter sp
            PC = opcode & 0x0FFF;	// Change l'adresse dans PC vers NNN
            break;

        case 0x3000: // Passe la prochaine instruction si VX == NN.
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                PC += 4;
            else
                PC += 2;
            break;

        case 0x4000: // Passe la prochaine instruction si VX != NN.
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                PC += 4;
            else
                PC += 2;
            break;

        case 0x5000: // 0x5XY0: Passe la prochaine instruction si if VX == VY.
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                PC += 4;
            else
                PC += 2;
            break;

        case 0x6000: // 0x6XNNN: Change la valeur de VX en NN.
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            PC += 2;
            break;

        case 0x7000: // 0x7XNN: Ajoute NN à VX.
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            PC += 2;
            break;

        case 0x8000: // Grand switch qui dépendra de la valeur des NNN

            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x8XY0: VX = VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0001: // 0x8XY1: VX = OU logique de VX et VY
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0002: // 0x8XY2: VX = ET logique de VX et VY
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0003: // 0x8XY3: VX = XOR de VX et VY
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0004: // 0x8XY4: VX += VY. VF (V[0xF]) sera égal à 1 si
                    V[opcode & 0x0F00 >> 8] += V[(opcode & 0x00F0) >> 4];
                    V[0xF] = (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) ? 1 : 0;
                    PC += 2;
                    break;

                case 0x0005: // 0x8XY5: VX -= VY.
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    V[0xF] = (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) ? 0 : 1;
                    PC += 2;
                    break;

                case 0x0006: // 0x8XY6: Shifts VX right by one.
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    PC += 2;
                    break;

                case 0x0007: // 0x8XY7: VX = VY - VX.
                    V[0xF] = (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) ? 0 : 1;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;
                case 0x000E:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    PC += 2;
                    break;
                default:
                    cerr << "Unkown opcode [0xE000]:0x" << hex << opcode << endl;
                    break;
            }

            break;

        case 0x9000: // 0x9XY0: Passe la prochaine instruction si VX != VY
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
                PC += 4;
            else
                PC += 2;
            break;

        case 0xA000: // Insère l'adresse NNN dans I
            I = opcode & 0x0FFF;
            PC += 2;
            break;

        case 0xB000: // Jumps à l'adresse NNN plus V0
            PC = (opcode & 0x0FFF) + V[0];
            break;

        case 0xC000: // CXNN: Valeur aléatoire dans VX & NN
            V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
            PC += 2;
            break;

        case 0xD000: // Pour le dessin
            V[0xF] = 0;
            unsigned short pixel;

            for (int y = 0; y < (opcode & 0x000F); y++)
            {
                pixel = memory[I + y];
                for(int x = 0; x < 8; x++)
                {
                    if((pixel & (0x80 >> x)) != 0)
                    {
                        if(gfx[(V[(opcode & 0x0F00) >> 8] + x + ((V[(opcode & 0x00F0) >> 4] + y) * 64))] == 1)
                            V[0xF] = 1;

                        gfx[V[(opcode & 0x0F00) >> 8] + x + ((V[(opcode & 0x00F0) >> 4] + y) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            PC += 2;

            break;

        case 0xE000: // Switch qui traitera selon si la touche est pressée ou pas
            switch(opcode & 0x00FF)
            {
                case 0x009E: // EX9E: Passe la prochaine instruction si la touche dans VX est pressée
                    if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                        PC += 4;
                    else
                        PC += 2;
                    break;

                case 0x00A1: // EXA1: Passe si la touche dans VX n'est pas pressée
                    if(key[V[(opcode & 0x0F00) >> 8]] == 0)
                        PC += 4;
                    else
                        PC += 2;
                    break;

                default:
                    cerr << "Unkown opcode [0xE000]:0x" << hex << opcode << endl;
                    break;
            }
            break;

        case 0xF000: // Timers et son
            switch(opcode & 0x00FF) {
                case 0x0007: // FX07
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    PC += 2;
                    break;

                case 0x000A: // FX0A
                {
                    bool keyPress = false;

                    for (int i = 0; i < 16; ++i) {
                        if (key[i] != 0) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            keyPress = true;
                        }
                    }

                    if (!keyPress)
                        return;

                    PC += 2;
                }

                case 0x0015: // FX15
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                case 0x0018: // FX18
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                case 0x001E: // FX1E
                    V[0xF] = (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) ? 1 : 0;
                    I += V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                case 0x0029: // FX29
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    PC += 2;
                    break;

                case 0x0033: // FX33
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
                    PC += 2;
                    break;

                case 0x0055: // FX55
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                        memory[I + i] = V[i];

                    I += ((opcode & 0x0F00) >> 8) + 1;
                    PC += 2;
                    break;

                case 0x0065: // FX65
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                        V[i] = memory[I + i];

                    I += ((opcode & 0x0F00) >> 8) + 1;
                    PC += 2;
                    break;

                default:
                    cerr << "Unkown opcode [0xF000]:0x" << hex << opcode << endl;
            }
            break;

        default:
            cerr << "Unkown opcode: 0x" << hex << opcode << endl;
            break;

    }

    if(delay_timer > 0)
        delay_timer--;

    if(sound_timer > 0)
        sound_timer--;

}




bool Chip8::loadApplication(const char * filename)
{
    initialize();
    FILE * file = NULL;
    if (!(file = fopen(filename, "rb")))
    {
        cerr << "Can't open file " << filename;
        return false;
    }

    fseek(file , 0 , SEEK_END);
    long size = ftell(file);
    errno = 0;
    rewind(file);
    if (errno != 0)
    {
        perror("rewind: ");
        return false;
    }

    cout << "File size " << size;

    char * buffer = (char*)malloc(size);
    if (!buffer)
    {
        cerr << "Memory allocation failed " << endl;
        return false;
    }

    size_t result = fread (buffer, 1, size, file);
    if (result != size)
    {
        cerr << "Reading error" << endl;
        return false;
    }

    if((4096-512) > size)
    {
        for(int i = 0; i < size; ++i)
            memory[i + 512] = buffer[i];
    }
    else
        cerr << "Error: Rom too big for memory" << endl;

    fclose(file);
    free(buffer);

    return true;



}





