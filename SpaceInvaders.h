#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <SDL2/SDL.h>
#include "8080.h"

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256

#define CYCLES_PER_FRAME (int)2e6 / 60 // 2Mhz at 60 fps

#define VRAM_ADDR 0x2400

typedef struct
{
    State *state8080;
    uint8_t shiftLSB;
    uint8_t shiftMSB;
    uint8_t shiftOffset;
    
    uint8_t port1;
    uint8_t port2;

    int interruptNum;

    uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH][4]; // RGBA format

} SpaceInvaders;

SpaceInvaders *initSpaceInvaders();
void runFrame(SpaceInvaders *si);
void updateBuffer(SpaceInvaders *si);