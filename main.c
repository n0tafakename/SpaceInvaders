#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <SDL2/SDL.h>
#include "SpaceInvaders.h"


// handles SDL keyboard events
void handleEvents(SpaceInvaders *si, SDL_Event *event, bool *done)
{
    while (SDL_PollEvent(event) != 0)
    {
        // do stuff
        switch (event->type)
        {   // shit hit the fan (or exit clicked)
            case SDL_QUIT: *done = true; break;

            // Key has been pressed
            case SDL_KEYDOWN:
            {
                switch (event->key.keysym.sym)
                {
                    // coin
                    case SDLK_c:
                    {
                        printf("c pressed\n");
                        si->port1 |= 0b00000001;
                    }

                    // 1 player game
                    case SDLK_RETURN:
                    {
                        si->port1 |= 0b00000100;
                        break;
                    }

                    // shoot
                    case SDLK_SPACE:
                    {
                        si->port1 |= 0b00010000;
                        break;
                    }

                    // left
                    case SDLK_LEFT:
                    {
                        si->port1 |= 0b00100000;
                        break;
                    }
                    
                    // right
                    case SDLK_RIGHT:
                    {
                        si->port1 |= 0b01000000;
                        break;
                    }

                    // tilt
                    case SDLK_t:
                    {
                        si->port2 |= 0b00000100;
                        break;
                    }
                }
                break;
            }

            // Key released
            case SDL_KEYUP:
            {
                switch (event->key.keysym.sym)
                {
                    // coin
                    case SDLK_c:
                    {
                        si->port1 &= ~(0b00000001);
                    }

                    // 1 player game
                    case SDLK_RETURN:
                    {
                        si->port1 &= ~(0b00000100);
                        break;
                    }

                    // shoot
                    case SDLK_SPACE:
                    {
                        si->port1 &= ~(0b00010000);
                        break;
                    }

                    // left
                    case SDLK_LEFT:
                    {
                        si->port1 &= ~(0b00100000);
                        break;
                    }
                    
                    // right
                    case SDLK_RIGHT:
                    {
                        si->port1 &= ~(0b01000000);
                        break;
                    }

                    // tilt
                    case SDLK_t:
                    {
                        si->port2 &= ~(0b00000100);
                        break;
                    }
                }
                break;
            }
        }
    }
}

// Interface between SDL and SI struct
void updateScreen(SpaceInvaders *si,  SDL_Texture *texture)
{
    const uint32_t pitch = sizeof(uint8_t) * 4 * SCREEN_WIDTH;
    SDL_UpdateTexture(texture, NULL, (si->screenBuffer), pitch);
}

// TODO: Put file and SDL stuff in subroutines
int main(int argc, char **argv)
{   
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL)
    {
        printf("File could not be opened\n");
        exit(EXIT_FAILURE);
    }

    int fd = fileno(f);
    struct stat st;

    /* get the size of the file */
    if (fstat(fd, &st) != 0)
        exit(EXIT_FAILURE);
    int size = st.st_size;

    /* write into the buffer */
    SpaceInvaders *spaceInvaders = initSpaceInvaders();
    fread(spaceInvaders->state8080->mem, size, 1, f);
    fclose(f);

    /* SDL initialization stuff */

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Shit hit the fan\n");
        exit(EXIT_FAILURE);
    }

    SDL_Window *window = SDL_CreateWindow("Nace Invaders", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 2,
                                          SCREEN_HEIGHT * 2, SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        printf("Shit hit the window\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowMinimumSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_ShowCursor(false);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (renderer == NULL)
    {
        printf("Shit hit the renderer\n");
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             SCREEN_WIDTH, SCREEN_HEIGHT);

    if (texture == NULL)
    {
        printf("Shit hit the texture\n");
        exit(EXIT_FAILURE);
    }

    
    /* Main routine */
    uint32_t timer = SDL_GetTicks(); // starting time
    bool done = false;
    SDL_Event event;
    while (!done)
    {
        handleEvents(spaceInvaders, &event, &done);

        // True every 1/60 seconds
        if (SDL_GetTicks() - timer > ((float)1 / 60) * (float)1000)
        {
            timer = SDL_GetTicks();
            runFrame(spaceInvaders);
            updateBuffer(spaceInvaders);
            updateScreen(spaceInvaders, texture);
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    free(spaceInvaders->state8080->codes);
    free(spaceInvaders->state8080->mem);
    free(spaceInvaders->state8080);
    free(spaceInvaders);
    return 0;
}