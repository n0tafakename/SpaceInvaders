#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "8080.h"

void out(State *state, uint8_t port)
{
    switch (port)
    {
        // lowest 3 bits set the shift offset
        case 2:
        {
            break;
        }

        case 4:
        {
            break;
        }
    }
    return;
}

void in(State *state, uint8_t port)
{
    switch (port)
    {
        // read the shifted value
        case 3:
        {
            break;
        }
    }
    return;
}

int emulateINOUT(State *state)
{
    unsigned char *code = &(state->mem[state->pc]);
    int success = 0;

    switch (code[0])
    {
        case 0xD3:
        {
            out(state, code[1]);
            success = 1;
            state->pc++;
            break;
        }

        case 0xDB:
        {
            in(state, code[1]);
            success = 1;
            state->pc++;
            break;
        }
    }

    return success;
}

// Reads in a memory file
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

    State *state = init8080();

    fread(state->mem, size, 1, f);
    fclose(f);

    // Main routine

    int done = 1;
    int print = 1;
    while (done < 75000)
    {
        if (!emulateINOUT(state))
            emulate8080(state, print);
        done++;
    }
    free(state->codes);
    free(state);
    return 0;
}