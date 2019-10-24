#include "SpaceInvaders.h"


static void out(SpaceInvaders *si, uint8_t port)
{
    switch (port)
    {
        // lowest 3 bits set the shift offset
        case 2:
        {
            si->shiftOffset = (si->state8080->a) & 0x7;
            break;
        }

        // Set shift register value
        case 4:
        {
            si->shiftLSB = si->shiftMSB;
            si->shiftMSB = si->state8080->a;
            break;
        }
    }
    return;
}

static void in(SpaceInvaders *si, uint8_t port)
{
    uint8_t val = 0;
    switch (port)
    {
        case 1:
        {
            val = si->port1;
            break;
        }

        case 2:
        {
            val = si->port2;
            break;
        }

        case 3:
        {
            uint16_t v = ((si->shiftMSB) << 8) | si->shiftLSB;
            val = ((v >> (8-(si->shiftOffset))) & 0xFF);
            break;
        }
    }

    si->state8080->a = val;
    return;
}

// returns 1 if IN or OUT is called
static int emulateINOUT(SpaceInvaders *si)
{
    unsigned char *code = &(si->state8080->mem[si->state8080->pc]);
    int success = 0;

    switch (code[0])
    {
        case 0xD3:
        {
            out(si, code[1]);
            success = 1;
            si->state8080->pc += 2;
            break;
        }

        case 0xDB:
        {
            in(si, code[1]);
            success = 1;
            si->state8080->pc += 2;
            break;
        }
    }

    return success;
}

SpaceInvaders *initSpaceInvaders()
{
    SpaceInvaders *new = malloc(sizeof(SpaceInvaders));
    new->state8080 = init8080();
    new->interruptNum = 1;
    new->shiftLSB = 0;
    new->shiftMSB = 0;
    new->shiftOffset = 0;
    new->port1 = 0;
    new->port2 = 0;
    return new;
}

void runFrame(SpaceInvaders *si)
{
    int cycles = 0;
    int interruptCycles = CYCLES_PER_FRAME / 2;

    while (cycles < CYCLES_PER_FRAME)
    {
        if (!emulateINOUT(si))
            cycles += emulate8080(si->state8080);
        
        else //update cycles for INOUT success
            cycles += 10;
        
        // Check if time for an interrupt
        if (cycles >= interruptCycles)
        {
            interruptCycles *= 2;
            GenerateInterrupt(si->state8080, si->interruptNum);
            si->interruptNum = (si->interruptNum & 1) ? 2 : 1;
        }
    }
}

void updateBuffer(SpaceInvaders *si)
{
    // Each byte holds 8 pixels of the screen
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH / 8; i++)
    {
        int y = i * 8 / SCREEN_HEIGHT;
        int x = (i * 8) % SCREEN_HEIGHT;
        uint8_t curr_byte = si->state8080->mem[VRAM_ADDR + i];

        // Go through each pixel in current byte
        for (int bit = 0; bit < 8; bit++)
        {
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
            
            int byte_x = x + bit;
            int byte_y = y;

            if (((curr_byte) >> bit) & 1)
            {
                r = 255;
                g = 255;
                b = 255;
            }

            int tmp = byte_x;
            byte_x = byte_y;
            byte_y = SCREEN_HEIGHT - (tmp + 1);

            si->screenBuffer[byte_y][byte_x][0] = r;
            si->screenBuffer[byte_y][byte_x][1] = g;
            si->screenBuffer[byte_y][byte_x][2] = b;
            si->screenBuffer[byte_y][byte_x][3] = (uint8_t)0;
        }
    }
}
