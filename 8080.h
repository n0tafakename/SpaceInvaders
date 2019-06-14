#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct
{
    uint8_t z;  // zero
    uint8_t c;  // carry
    uint8_t p;  // parity
    uint8_t s;  // sign
    uint8_t ac; // auxiliary carry
} Codes;

typedef struct
{
    uint16_t pc;
    uint16_t sp;
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    bool int_en; // interrupt enable
    uint8_t *mem;
    Codes *codes;
} State;

int emulate8080(State *state, int print);
State *init8080();