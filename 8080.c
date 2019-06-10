#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>


// Holds values of flags (could be stored in one 8-bit word)
typedef struct
{
    uint8_t z; // zero
    uint8_t c; // carry
    uint8_t p; // parity set when even
    uint8_t s; // sign
    uint8_t ac; // half carry (auxiliary carry), bit 4 is set
} Codes;

// Holds registers and CCs (also a register), also memory and int_en
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
    bool int_en; // interrupt enable (INTE flip-flop)
    uint8_t *mem;
    Codes *codes;
} State;

// Reads in machine code and prints out 8080 assembly code
int disassemble8080(unsigned char *buf, unsigned int pc)
{
    unsigned char *code = &buf[pc];
    int8_t length = 1;

    printf("%04x ", pc);

    switch (code[0])
    {
        /* 1 byte codes */

        // MISC
        case 0x00: {
            printf("NOP");
            break;
        }

        case 0x76: {
            printf("HLT");
            break;
        }

        case 0xF3: {
            printf("DI");
            break;
        }

        case 0xFB: {
            printf("EI");
            break;
        }

        case 0xF9: {
            printf("SPHL");
            break;
        }

        case 0xE3: {
            printf("XTHL");
            break;
        }

        case 0xE9: {
            printf("PCHL");
            break;
        }

        // RST
        case 0xC7: printf("RST    0"); break;
        case 0xCF: printf("RST    1"); break;
        case 0xD7: printf("RST    2"); break;
        case 0xDF: printf("RST    3"); break;
        case 0xE7: printf("RST    4"); break;
        case 0xEF: printf("RST    5"); break;
        case 0xF7: printf("RST    6"); break;
        case 0xFF: printf("RST    7"); break;

        // Rccc
        case 0xC0: printf("RNZ"); break;
        case 0xC8: printf("RZ"); break;
        case 0xD0: printf("RNC"); break;
        case 0xD8: printf("RC"); break;
        case 0xE0: printf("RPO"); break;
        case 0xE8: printf("RPE"); break;
        case 0xF0: printf("RP"); break;
        case 0xF8: printf("RM"); break;

        // MISC
        case 0xC9: {
            printf("RET");
            break;
        }

        case 0x37: {
            printf("STC");
            break;
        }

        case 0x3F: {
            printf("CMC");
            break;
        }

        case 0x2F: {
            printf("CMA");
            break;
        }

        case 0x1F: {
            printf("RAR");
            break;
        }

        case 0x17: {
            printf("RAL");
            break;
        }

        case 0x0F: {
            printf("RRC");
            break;
        }

        case 0x07: {
            printf("RLC");
            break;
        }

        case 0x27: {
            printf("DAA");
            break;
        }

        case 0xEB: {
            printf("XCHG");
            break;
        }

        // Might not be correct (Using double names, not single) Does that matter?
        case 0x0A: printf("LDAX    BC"); break;
        case 0x1A: printf("LDAX    DE"); break;

        case 0x02: printf("STAX    BC"); break;
        case 0x12: printf("STAX    DE"); break;

        case 0xC1: printf("POP    BC"); break;
        case 0xD1: printf("POP    DE"); break;
        case 0xE1: printf("POP    HL"); break;
        case 0xF1: printf("POP    PSW"); break;

        case 0xC5: printf("PUSH    BC"); break;
        case 0xD5: printf("PUSH    DE"); break;
        case 0xE5: printf("PUSH    HL"); break;
        case 0xF5: printf("PUSH    PSW"); break;

        // MOV
        case 0x40: printf("MOV    B,B"); break;
		case 0x41: printf("MOV    B,C"); break;
		case 0x42: printf("MOV    B,D"); break;
		case 0x43: printf("MOV    B,E"); break;
		case 0x44: printf("MOV    B,H"); break;
		case 0x45: printf("MOV    B,L"); break;
		case 0x46: printf("MOV    B,M"); break;
		case 0x47: printf("MOV    B,A"); break;
		case 0x48: printf("MOV    C,B"); break;
		case 0x49: printf("MOV    C,C"); break;
		case 0x4a: printf("MOV    C,D"); break;
		case 0x4b: printf("MOV    C,E"); break;
		case 0x4c: printf("MOV    C,H"); break;
		case 0x4d: printf("MOV    C,L"); break;
		case 0x4e: printf("MOV    C,M"); break;
		case 0x4f: printf("MOV    C,A"); break;	
		case 0x50: printf("MOV    D,B"); break;
		case 0x51: printf("MOV    D,C"); break;
		case 0x52: printf("MOV    D,D"); break;
		case 0x53: printf("MOV    D.E"); break;
		case 0x54: printf("MOV    D,H"); break;
		case 0x55: printf("MOV    D,L"); break;
		case 0x56: printf("MOV    D,M"); break;
		case 0x57: printf("MOV    D,A"); break;
		case 0x58: printf("MOV    E,B"); break;
		case 0x59: printf("MOV    E,C"); break;
		case 0x5a: printf("MOV    E,D"); break;
		case 0x5b: printf("MOV    E,E"); break;
		case 0x5c: printf("MOV    E,H"); break;
		case 0x5d: printf("MOV    E,L"); break;
		case 0x5e: printf("MOV    E,M"); break;
		case 0x5f: printf("MOV    E,A"); break;
		case 0x60: printf("MOV    H,B"); break;
		case 0x61: printf("MOV    H,C"); break;
		case 0x62: printf("MOV    H,D"); break;
		case 0x63: printf("MOV    H.E"); break;
		case 0x64: printf("MOV    H,H"); break;
		case 0x65: printf("MOV    H,L"); break;
		case 0x66: printf("MOV    H,M"); break;
		case 0x67: printf("MOV    H,A"); break;
		case 0x68: printf("MOV    L,B"); break;
		case 0x69: printf("MOV    L,C"); break;
		case 0x6a: printf("MOV    L,D"); break;
		case 0x6b: printf("MOV    L,E"); break;
		case 0x6c: printf("MOV    L,H"); break;
		case 0x6d: printf("MOV    L,L"); break;
		case 0x6e: printf("MOV    L,M"); break;
		case 0x6f: printf("MOV    L,A"); break;
		case 0x70: printf("MOV    M,B"); break;
		case 0x71: printf("MOV    M,C"); break;
		case 0x72: printf("MOV    M,D"); break;
		case 0x73: printf("MOV    M.E"); break;
		case 0x74: printf("MOV    M,H"); break;
		case 0x75: printf("MOV    M,L"); break;
		case 0x77: printf("MOV    M,A"); break;
		case 0x78: printf("MOV    A,B"); break;
		case 0x79: printf("MOV    A,C"); break;
		case 0x7a: printf("MOV    A,D"); break;
		case 0x7b: printf("MOV    A,E"); break;
		case 0x7c: printf("MOV    A,H"); break;
		case 0x7d: printf("MOV    A,L"); break;
		case 0x7e: printf("MOV    A,M"); break;
		case 0x7f: printf("MOV    A,A"); break;

        // ADD/ADC
        case 0x80: printf("ADD    B"); break;
		case 0x81: printf("ADD    C"); break;
		case 0x82: printf("ADD    D"); break;
		case 0x83: printf("ADD    E"); break;
		case 0x84: printf("ADD    H"); break;
		case 0x85: printf("ADD    L"); break;
		case 0x86: printf("ADD    M"); break;
		case 0x87: printf("ADD    A"); break;
		case 0x88: printf("ADC    B"); break;
		case 0x89: printf("ADC    C"); break;
		case 0x8a: printf("ADC    D"); break;
		case 0x8b: printf("ADC    E"); break;
		case 0x8c: printf("ADC    H"); break;
		case 0x8d: printf("ADC    L"); break;
		case 0x8e: printf("ADC    M"); break;
		case 0x8f: printf("ADC    A"); break;

        // SUB/SBB
        case 0x90: printf("SUB    B"); break;
		case 0x91: printf("SUB    C"); break;
		case 0x92: printf("SUB    D"); break;
		case 0x93: printf("SUB    E"); break;
		case 0x94: printf("SUB    H"); break;
		case 0x95: printf("SUB    L"); break;
		case 0x96: printf("SUB    M"); break;
		case 0x97: printf("SUB    A"); break;
		case 0x98: printf("SBB    B"); break;
		case 0x99: printf("SBB    C"); break;
		case 0x9a: printf("SBB    D"); break;
		case 0x9b: printf("SBB    E"); break;
		case 0x9c: printf("SBB    H"); break;
		case 0x9d: printf("SBB    L"); break;
		case 0x9e: printf("SBB    M"); break;
		case 0x9f: printf("SBB    A"); break;

        // ANA
		case 0xa0: printf("ANA    B"); break;
		case 0xa1: printf("ANA    C"); break;
		case 0xa2: printf("ANA    D"); break;
		case 0xa3: printf("ANA    E"); break;
		case 0xa4: printf("ANA    H"); break;
		case 0xa5: printf("ANA    L"); break;
		case 0xa6: printf("ANA    M"); break;
		case 0xa7: printf("ANA    A"); break;
		
        // XRA
        case 0xa8: printf("XRA    B"); break;
		case 0xa9: printf("XRA    C"); break;
		case 0xaa: printf("XRA    D"); break;
		case 0xab: printf("XRA    E"); break;
		case 0xac: printf("XRA    H"); break;
		case 0xad: printf("XRA    L"); break;
		case 0xae: printf("XRA    M"); break;
		case 0xaf: printf("XRA    A"); break;

        // ORA
		case 0xb0: printf("ORA    B"); break;
		case 0xb1: printf("ORA    C"); break;
		case 0xb2: printf("ORA    D"); break;
		case 0xb3: printf("ORA    E"); break;
		case 0xb4: printf("ORA    H"); break;
		case 0xb5: printf("ORA    L"); break;
		case 0xb6: printf("ORA    M"); break;
		case 0xb7: printf("ORA    A"); break;
		
        // CMP
        case 0xb8: printf("CMP    B"); break;
		case 0xb9: printf("CMP    C"); break;
		case 0xba: printf("CMP    D"); break;
		case 0xbb: printf("CMP    E"); break;
		case 0xbc: printf("CMP    H"); break;
		case 0xbd: printf("CMP    L"); break;
		case 0xbe: printf("CMP    M"); break;
		case 0xbf: printf("CMP    A"); break;

        // INX
        case 0x03: printf("INX    BC"); break;
        case 0x13: printf("INX    DE"); break;
        case 0x23: printf("INX    HL"); break;
        case 0x33: printf("INX    SP"); break;

        // DCX
        case 0x0B: printf("DCX    BC"); break;
        case 0x1B: printf("DCX    DE"); break;
        case 0x2B: printf("DCX    HL"); break;
        case 0x3B: printf("DCX    SP"); break;

        // DAD
        case 0x09: printf("DAD    BC"); break;
        case 0x19: printf("DAD    DE"); break;
        case 0x29: printf("DAD    HL"); break;
        case 0x39: printf("DAD    SP"); break;
        
        // INR
        case 0x04: printf("INR    B"); break;
        case 0x0C: printf("INR    C"); break;
        case 0x14: printf("INR    D"); break;
        case 0x1C: printf("INR    E"); break;
        case 0x24: printf("INR    H"); break;
        case 0x2C: printf("INR    L"); break;
        case 0x34: printf("INR    M"); break;
        case 0x3C: printf("INR    A"); break;

        // DCR
        case 0x05: printf("DCR    B"); break;
        case 0x0D: printf("DCR    C"); break;
        case 0x15: printf("DCR    D"); break;
        case 0x1D: printf("DCR    E"); break;
        case 0x25: printf("DCR    H"); break;
        case 0x2D: printf("DCR    L"); break;
        case 0x35: printf("DCR    M"); break;
        case 0x3D: printf("DCR    A"); break;

        /* 2 byte codes */

        // MISC
        case 0xD3: printf("OUT    #$%02x", code[1]); length = 2; break;
        case 0xDB: printf("IN    #$%02x", code[1]); length = 2; break;
        case 0xFE: printf("CPI    #$%02x", code[1]); length = 2; break;
        case 0xEE: printf("XRI    #$%02x", code[1]); length = 2; break;
        case 0xF6: printf("ORI    #$%02x", code[1]); length = 2; break;
        case 0xE6: printf("ANI    #$%02x", code[1]); length = 2; break;
        case 0xDE: printf("SBI    #$%02x", code[1]); length = 2; break;
        case 0xD6: printf("SUI    #$%02x", code[1]); length = 2; break;
        case 0xCE: printf("ACI    #$%02x", code[1]); length = 2; break;
        case 0xC6: printf("ADI    #$%02x", code[1]); length = 2; break;

        // MVI
        case 0x06: printf("MVI    B,#$%02x", code[1]); length = 2; break;
        case 0x0E: printf("MVI    C,#$%02x", code[1]); length = 2; break;
        case 0x16: printf("MVI    D,#$%02x", code[1]); length = 2; break;
        case 0x1E: printf("MVI    E,#$%02x", code[1]); length = 2; break;
        case 0x26: printf("MVI    H,#$%02x", code[1]); length = 2; break;
        case 0x2E: printf("MVI    L,#$%02x", code[1]); length = 2; break;
        case 0x36: printf("MVI    M,#$%02x", code[1]); length = 2; break;
        case 0x3E: printf("MVI    A,#$%02x", code[1]); length = 2; break;

        /* 3 byte codes */

        // MISC
        case 0x3A: printf("LDA    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0x32: printf("STA    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0x22: printf("SHLD    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0x2A: printf("LHLD    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xC3: printf("JMP    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xCD: printf("CALL    $%02x%02x", code[2], code[1]); length = 3; break;

        // LXI
        case 0x01: printf("LXI    BC,#$%02x%02x", code[2], code[1]); length = 3; break;
        case 0x11: printf("LXI    DE,#$%02x%02x", code[2], code[1]); length = 3; break;
        case 0x21: printf("LXI    HL,#$%02x%02x", code[2], code[1]); length = 3; break;
        case 0x31: printf("LXI    SP,#$%02x%02x", code[2], code[1]); length = 3; break;

        // Jccc
        case 0xC2: printf("JNZ    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xCA: printf("JZ    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xD2: printf("JNC    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xDA: printf("JC    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xE2: printf("JPO    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xEA: printf("JPE    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xF2: printf("JP    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xFA: printf("JM    $%02x%02x", code[2], code[1]); length = 3; break;

        // Cccc
        case 0xC4: printf("CNZ    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xCC: printf("CZ    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xD4: printf("CNC    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xDC: printf("CC    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xE4: printf("CPO    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xEC: printf("CPE    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xF4: printf("CP    $%02x%02x", code[2], code[1]); length = 3; break;
        case 0xFC: printf("CM    $%02x%02x", code[2], code[1]); length = 3; break;
    }

    printf("\n");

    return length;
}

// Error message for unimplemented instructions (add more info later)
int UnimplementedInstruction(unsigned int instruction)
{
    printf("Well... shit...\n");
    printf("%02x is not implemented (either by me or the program is fucked)", instruction);
    exit(EXIT_FAILURE);
}

// Returns 1 if the number bits set is even
int parity(uint8_t num)
{
    uint8_t tmp1, tmp2;
    tmp1 = num ^ (num >> 1);
    tmp2 = tmp1 ^ (tmp1 >> 2);
    return ((tmp2 ^ (tmp2 >> 4)) & 1) ? 0 : 1;
}

// Set flags based on the result of the operation
// TODO: figure out auxiliary carry
void setArithFlags(State *state, uint16_t result)
{
    // carry
    state->codes->c = ((result & 0x100) == 0x100);

    // zero
    state->codes->z = ((result & 0xFF) == 0);

    // sign
    state->codes->s = ((result & 0x80) == 0x80);

    // parity
    state->codes->p = parity((uint8_t)(result & 0xFF));

}

// Sets every flag except for the carry flag
void setAllButCarry(State *state, uint16_t result)
{
    // zero
    state->codes->z = ((result & 0xFF) == 0);

    // sign
    state->codes->s = ((result & 0x80) == 0x80);

    // parity
    state->codes->p = parity((uint8_t)(result & 0xFF));
}

// Set the carry flag based on result
void setCarry(State *state, uint16_t result)
{
    state->codes->c = ((result & 0x100) == 0x100);
}

// Adds num to register A
void add(State *state, uint16_t num, bool carry)
{
    uint16_t tmp = (uint16_t)state->a + num;
    if (carry) tmp++;

    // Set AC flag
    if (((state->a & 0xF) + (num & 0xF)) > 0xF)
        state->codes->ac = 0x1;

    state->a = (uint8_t)tmp;

    // Set flags
    setArithFlags(state, tmp);
}

// Subtracts num from register A
void sub(State *state, uint16_t num, bool borrow)
{
    uint16_t tmp = (uint16_t)state->a - num;
    if (borrow) tmp++;
    state->a = (uint8_t)tmp;

    // Set AC flag
    if (((state->a & 0xF0) - (num & 0xF0)) != ((state->a - num) & 0xF0))
        state->codes->ac = 0x1;

    // Set flags
    setArithFlags(state, tmp);
}

// And num with register A
void and(State *state, uint16_t num)
{
    uint16_t tmp = (uint16_t)state->a & num;
    state->a = (uint8_t)tmp;

    // Set flags
    setArithFlags(state, tmp);
}

// Or num with register A
void or(State *state, uint16_t num)
{
    uint16_t tmp = (uint16_t)state->a | num;
    state->a = (uint8_t)tmp;

    // Set flags
    setArithFlags(state, tmp);
}

// Xor num with register A
void xor(State *state, uint16_t num)
{
    uint16_t tmp = (uint16_t)state->a ^ num;
    state->a = (uint8_t)tmp;

    // Set flags
    setArithFlags(state, tmp);
}

// Set flags according to the result of a - num
void cmp(State *state, uint16_t num)
{
    uint16_t tmp = (uint16_t)state->a - num;

    // Set AC flag
    if (((state->a & 0xF0) - (num & 0xF0)) != ((state->a - num) & 0xF0))
        state->codes->ac = 0x1;

    // Set flags
    setArithFlags(state, tmp);

    // Flip carry flag if sign does not match
    if ((state->a >> 7) != (((uint8_t)num) >> 7))
        state->codes->c = !(state->codes->c);

}

// Increment register value
void inr(State *state, uint8_t *value)
{
    uint16_t tmp = 1 + (uint16_t)(*value);

    // Set AC flag
    if (((*value & 0xF) + 1) < 0xF)
        state->codes->ac = 0x1;

    *value = (uint8_t)tmp;
    
    // Set flags
    setAllButCarry(state, tmp);
}

// Decrement register value
void dcr(State *state, uint8_t *value)
{
    uint16_t tmp = (uint16_t)(*value) - 1;

    if ((*value & 0xF) == 0)
        state->codes->ac = 0x1;

    *value = (uint8_t)tmp;
    
    // Set flags
    setAllButCarry(state, tmp);
}

// Set the destination (reg or mem) to the value of source (reg or mem)
void mov(uint8_t *dest, uint8_t *src)
{
    *dest = *src;
}

// Increment register pair
void inx(uint8_t *hi, uint8_t *lo)
{
    uint16_t tmp = ((uint16_t)(*hi) << 8) | (uint16_t)(*lo);
    tmp++;
    *hi = (uint8_t)(tmp >> 8);
    *lo = (uint8_t)(tmp & 0xFF);
}

// Decrement register pair
void dcx(uint8_t *hi, uint8_t *lo)
{
    uint16_t tmp = ((uint16_t)(*hi) << 8) | (uint16_t)(*lo);
    tmp--;
    *hi = (uint8_t)(tmp >> 8);
    *lo = (uint8_t)(tmp & 0xFF);
}

// Add value to value stored in mem at HL
void dad(State* state, uint16_t value)
{

    uint16_t new = ((uint16_t)state->h << 8) | (uint16_t)state->l;
    uint32_t tmp = new + value;
    new += value; 
    state->h = (uint8_t)(new >> 8);
    state->l = (uint8_t)(new & 0xFF);
    if (tmp >> 16) // Set carry
        state->codes->c = 0x1;
}

// Jump to new address if condition is true, otherwise continue execution
void jump(State* state, uint8_t cond, uint16_t addr)
{
    if (cond == (uint8_t)1)
        state->pc = addr;
    else
        state->pc += 2;
}

// Push register pair onto the stack
void push(State *state, uint8_t hi, uint8_t lo)
{
    state->sp--;
    state->mem[state->sp] = hi;
    state->sp--;
    state->mem[state->sp] = lo;
    //printf("\n\nPushed on: %02x%02x\n\n", state->mem[state->sp + 1], state->mem[state->sp]);
}

void pop(State *state, uint8_t *hi, uint8_t *lo)
{
    *lo = state->mem[state->sp];
    state->sp++;
    *hi = state->mem[state->sp];
    state->sp++;
    //printf("\n\nPopped off: %02x%02x\n\n", *hi, *lo);
    //printf("Stack view of popped: %02x%02x\n\n", state->mem[state->sp - 1], state->mem[state->sp - 2]);
    //printf("2nd on stack: %02x%02x\n\n", state->mem[state->sp - 3], state->mem[state->sp - 4]);
    //printf("Above stack: %02x%02x\n\n", state->mem[state->sp + 1], state->mem[state->sp]);
}

void call(State *state, uint8_t cond, uint16_t addr)
{
    // push return address
    state->pc += 2;
    uint8_t hi = (state->pc >> 8) & 0xFF;
    uint8_t lo = state->pc & 0xFF;
    push(state, hi, lo);

    // White Men Can't Jump (except in this case)
    jump(state, cond, addr);
}

void ret(State* state)
{
    uint8_t hi, lo;
    pop(state, &hi, &lo);
    state->pc = (uint16_t)(hi << 8) | (uint16_t)lo;
}

void stackPeek(State *state)
{
    printf("Above %02x%02x\n\n", state->mem[state->sp + 3], state->mem[state->sp + 2]);
    printf("Top of stack: %02x%02x\n\n", state->mem[state->sp + 1], state->mem[state->sp]);
    printf("Below: %02x%02x\n\n", state->mem[state->sp - 1], state->mem[state->sp - 1]);
}

// Use the buf full of data and update the current state based on the instruction
int emulate8080(State *state)
{
    unsigned char *code = &(state->mem[state->pc]);
    // printf("0x%04x", state->pc);
    disassemble8080(state->mem, state->pc);
    state->pc++;
    
    printf("\t");
	printf("%c", state->codes->z ? 'z' : '.');
	printf("%c", state->codes->s ? 's' : '.');
	printf("%c", state->codes->p ? 'p' : '.');
	printf("%c", state->codes->c ? 'c' : '.');
	printf("%c  ", state->codes->ac ? 'a' : '.');
	printf("A $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n", state->a, state->b, state->c,
				state->d, state->e, state->h, state->l, state->sp);
    
    stackPeek(state);

    switch (code[0])
    {
        /* 1 byte codes */

        // NOP
        case 0x00: break;

        // go to next instruction and wait until interrupt is received
        // Since there is no ISR, just exit (will change later)
        // HLT
        case 0x76: exit(EXIT_SUCCESS); break;

        // DI
        case 0xF3: state->int_en = false; break;

        // EI
        case 0xFB: state->int_en = true; break;

        // SPHL
        case 0xF9:
        {
            state->sp = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            break;
        }

        // XTHL
        case 0xE3:
        {
            uint8_t loStackVal = state->mem[state->sp];
            uint8_t hiStackVal = state->mem[(state->sp)+1];
            uint8_t lVal = state->l;
            uint8_t hVal = state->h;

            state->mem[state->sp] = lVal;
            state->mem[(state->sp)+1] = hVal;
            state->l = loStackVal;
            state->h = hiStackVal;
            break;
        }

        // PCHL
        // Set pc to HL
        case 0xE9:
        {
            state->pc = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            break;
        }

        // RST (push return onto stack and jump to 0b0000_0000_00xx_x000)
        case 0xC7: call(state, 0x1, 0x0000); break;
        case 0xCF: call(state, 0x1, 0x0008); break;
        case 0xD7: call(state, 0x1, 0x0010); break;
        case 0xDF: call(state, 0x1, 0x0018); break;
        case 0xE7: call(state, 0x1, 0x0020); break;
        case 0xEF: call(state, 0x1, 0x0028); break;
        case 0xF7: call(state, 0x1, 0x0030); break;
        case 0xFF: call(state, 0x1, 0x0038); break;

        // Rccc
        case 0xC0:
        {
            if (!(state->codes->z))
                ret(state);
            break;
        }
        case 0xC8:
        {
            if ((state->codes->z))
                ret(state);
            break;
        }
        case 0xD0:
        {
            if (!(state->codes->c))
                ret(state);
            break;
        }
        case 0xD8:
        {
            if ((state->codes->c))
                ret(state);
            break;
        }
        case 0xE0:
        {
            if (!(state->codes->p))
                ret(state);
            break;
        }
        case 0xE8:
        {
            if ((state->codes->p))
                ret(state);
            break;
        }
        case 0xF0:
        {
            if (!(state->codes->s))
                ret(state);
            break;
        }
        case 0xF8:
        {
            if ((state->codes->s))
                ret(state);
            break;
        }

        // RET
        case 0xC9: ret(state); break;

        // STC
        case 0x37: state->codes->c = 1; break;

        // CMC
        case 0x3F: state->codes->c = !(state->codes->c); break;

        // CMA
        case 0x2F: state->a = ~(state->a); break;


        /*
        No masks are used on the bit shifts
        because all types are unsigned (no extension)
        */

        // RAR
        case 0x1F:
        {
            uint8_t carry = state->codes->c;
            uint8_t lo = state->a & 0x1;

            state->a = (state->a >> 1) | (carry << 7);
            state->codes->c = lo;
            break;
        }

        // RAL
        case 0x17:
        {
            uint8_t carry = state->codes->c;
            uint8_t hi = (state->a) >> 7;
            
            state->a = ((state->a) << 1) | carry;
            state->codes->c = hi;
            break;
        }

        // RRC
        case 0x0F:
        {
            state->codes->c = state->a & 0x1;
            state->a = (state->a >> 1) | (state->a << 7);
            break;
        }

        // RLC
        case 0x07:
        {
            state->codes->c = (state->a) >> 7;
            state->a = (state->a << 1) | ((state->a) >> 7);
            break;
        }

        // DAA
        case 0x27:
        {
            if ((((state->a) & 0xF) > 0x9) || state->codes->ac)
                state->a += 0x6;
            
            if ((((state->a) & 0xF0) > 0x90) || state->codes->c)
                state->a += 0x60;
            
            // Set Flags
            if (((state->a) & 0xF) < 0x6)
                state->codes->ac = 0x1;
            
            if (((state->a) & 0xF0) < 0x60)
                state->codes->c = 0x1;
            
            setAllButCarry(state, (uint16_t)state->a);
            break;
        }

        // XCHG
        case 0xEB:
        {
            uint8_t hi = state->h;
            uint8_t lo = state->l;
            uint8_t d = state->d;
            uint8_t e = state->e;
            state->h = d;
            state->l = e;
            state->d = hi;
            state->e = lo;
            break;
        }

        // LDAX
        case 0x0A:
        {
            uint16_t addr = ((uint16_t)state->b << 8) | (uint16_t)state->c;
            state->a = state->mem[addr];
            break;
        }
        case 0x1A:
        {
            printf("val before: %x\n",state->mem[state->sp]);
            uint16_t addr = ((uint16_t)state->d << 8) | (uint16_t)state->e;
            state->a = state->mem[addr];
            printf("val after: %x\n",state->mem[state->sp]);
            break;
        }

        // STAX
        case 0x02:
        {
            uint16_t addr = ((uint16_t)state->b << 8) | (uint16_t)state->c;
            state->mem[addr] = state->a;
            break;
        }
        case 0x12:
        {
            uint16_t addr = ((uint16_t)state->d << 8) | (uint16_t)state->e;
            state->mem[addr] = state->a;
            break;
        }

        // POP
        case 0xC1: pop(state, &(state->b), &(state->c)); break;
        case 0xD1: pop(state, &(state->d), &(state->e)); break;
        case 0xE1: pop(state, &(state->h), &(state->l)); break;
        // This one affects all flags
        case 0xF1:
        {
            state->a = state->mem[state->sp];
            state->sp++;
            uint8_t psw = state->mem[state->sp];
            state->sp++;
            
            // restore condition codes
            state->codes->c = (psw & 0x1);
            state->codes->p = (psw >> 2) & 0x1;
            state->codes->ac = (psw >> 4) & 0x1;
            state->codes->z = (psw >> 6) & 0x1;
            state->codes->s = (psw >> 7) & 0x1;
        }

        // PUSH
        case 0xC5: push(state, state->b, state->c); break;
        case 0xD5: push(state, state->d, state->e); break;
        case 0xE5: push(state, state->h, state->l); break;
        case 0xF5:
        {
            uint8_t ccs = (state->codes->c) | 0x2 | (state->codes->p << 2)
                        | (state->codes->ac << 4) | (state->codes->z << 6)
                        | (state->codes->s << 7);
            push(state, state->a, ccs);
            break;
        }

        // MOV
        case 0x40: mov(&(state->b), &(state->b)); break;
		case 0x41: mov(&(state->b), &(state->c)); break;
		case 0x42: mov(&(state->b), &(state->d)); break;
		case 0x43: mov(&(state->b), &(state->e)); break;
		case 0x44: mov(&(state->b), &(state->h)); break;
		case 0x45: mov(&(state->b), &(state->l)); break;
		case 0x46: 
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->b), &(state->mem[addr]));
            break;
        }
		case 0x47: mov(&(state->b), &(state->a)); break;
		case 0x48: mov(&(state->c), &(state->b)); break;
		case 0x49: mov(&(state->c), &(state->c)); break;
		case 0x4a: mov(&(state->c), &(state->d)); break;
		case 0x4b: mov(&(state->c), &(state->e)); break;
		case 0x4c: mov(&(state->c), &(state->h)); break;
		case 0x4d: mov(&(state->c), &(state->l)); break;
		case 0x4e: 
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->c), &(state->mem[addr]));
            break;
        }
		case 0x4f: mov(&(state->c), &(state->a)); break;
		case 0x50: mov(&(state->d), &(state->b)); break;
		case 0x51: mov(&(state->d), &(state->c)); break;
		case 0x52: mov(&(state->d), &(state->d)); break;
		case 0x53: mov(&(state->d), &(state->e)); break;
		case 0x54: mov(&(state->d), &(state->h)); break;
		case 0x55: mov(&(state->d), &(state->l)); break;
		case 0x56:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->d), &(state->mem[addr]));
            break;
        }
		case 0x57: mov(&(state->d), &(state->a)); break;
		case 0x58: mov(&(state->e), &(state->b)); break;
		case 0x59: mov(&(state->e), &(state->c)); break;
		case 0x5a: mov(&(state->e), &(state->d)); break;
		case 0x5b: mov(&(state->e), &(state->e)); break;
		case 0x5c: mov(&(state->e), &(state->h)); break;
		case 0x5d: mov(&(state->e), &(state->l)); break;
		case 0x5e:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->e), &(state->mem[addr]));
            break;
        }
		case 0x5f: mov(&(state->e), &(state->a)); break;
		case 0x60: mov(&(state->h), &(state->b)); break;
		case 0x61: mov(&(state->h), &(state->c)); break;
		case 0x62: mov(&(state->h), &(state->d)); break;
		case 0x63: mov(&(state->h), &(state->e)); break;
		case 0x64: mov(&(state->h), &(state->h)); break;
		case 0x65: mov(&(state->h), &(state->l)); break;
		case 0x66:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->h), &(state->mem[addr]));
            break;
        }
		case 0x67: mov(&(state->h), &(state->a)); break;
		case 0x68: mov(&(state->l), &(state->b)); break;
		case 0x69: mov(&(state->l), &(state->c)); break;
		case 0x6a: mov(&(state->l), &(state->d)); break;
		case 0x6b: mov(&(state->l), &(state->e)); break;
		case 0x6c: mov(&(state->l), &(state->h)); break;
		case 0x6d: mov(&(state->l), &(state->l)); break;
		case 0x6e:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->l), &(state->mem[addr]));
            break;
        }
		case 0x6f: mov(&(state->l), &(state->a)); break;
		case 0x70:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(state->b));
            break;
        }
		case 0x71:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(state->c));
            break;
        }
		case 0x72:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(state->d));
            break;
        }
		case 0x73:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(state->e));
            break;
        }
		case 0x74:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(state->h));
            break;
        }
		case 0x75:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(state->l));
            break;
        }
		case 0x77:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(state->a));
            break;
        }
		case 0x78: mov(&(state->a), &(state->b)); break;
		case 0x79: mov(&(state->a), &(state->c)); break;
		case 0x7a: mov(&(state->a), &(state->d)); break;
		case 0x7b: mov(&(state->a), &(state->e)); break;
		case 0x7c: mov(&(state->a), &(state->h)); break;
		case 0x7d: mov(&(state->a), &(state->l)); break;
		case 0x7e:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->a), &(state->mem[addr]));
            break;
        }
		case 0x7f: mov(&(state->a), &(state->a)); break;

        // ADD/ADC
        case 0x80: add(state, (uint16_t)state->b, false); break;
		case 0x81: add(state, (uint16_t)state->c, false); break;
		case 0x82: add(state, (uint16_t)state->d, false); break;
		case 0x83: add(state, (uint16_t)state->e, false); break;
		case 0x84: add(state, (uint16_t)state->h, false); break;
		case 0x85: add(state, (uint16_t)state->l, false); break;
        case 0x86:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            add(state, (uint16_t)state->mem[addr], false);
            break;
        }
		case 0x87: add(state, (uint16_t)state->a, false); break;
		case 0x88: add(state, (uint16_t)state->b, true); break;
		case 0x89: add(state, (uint16_t)state->c, true); break;
		case 0x8a: add(state, (uint16_t)state->d, true); break;
		case 0x8b: add(state, (uint16_t)state->e, true); break;
		case 0x8c: add(state, (uint16_t)state->h, true); break;
		case 0x8d: add(state, (uint16_t)state->l, true); break;
		case 0x8e:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            add(state, (uint16_t)state->mem[addr], true);
            break;
        }
		case 0x8f: add(state, (uint16_t)state->a, true); break;

        // SUB/SBB
        case 0x90: sub(state, (uint16_t)state->b, false); break;
		case 0x91: sub(state, (uint16_t)state->c, false); break;
		case 0x92: sub(state, (uint16_t)state->d, false); break;
		case 0x93: sub(state, (uint16_t)state->e, false); break;
		case 0x94: sub(state, (uint16_t)state->h, false); break;
		case 0x95: sub(state, (uint16_t)state->l, false); break;
		case 0x96:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            sub(state, (uint16_t)state->mem[addr], false);
            break;
        }
		case 0x97: sub(state, (uint16_t)state->a, false); break;
		case 0x98: sub(state, (uint16_t)state->b, true); break;
		case 0x99: sub(state, (uint16_t)state->c, true); break;
		case 0x9a: sub(state, (uint16_t)state->d, true); break;
		case 0x9b: sub(state, (uint16_t)state->e, true); break;
		case 0x9c: sub(state, (uint16_t)state->h, true); break;
		case 0x9d: sub(state, (uint16_t)state->l, true); break;
		case 0x9e:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            sub(state, (uint16_t)state->mem[addr], true);
            break;
        }
		case 0x9f: sub(state, (uint16_t)state->a, true); break;

        // ANA
		case 0xa0: and(state, (uint16_t)state->b); break;
		case 0xa1: and(state, (uint16_t)state->c); break;
		case 0xa2: and(state, (uint16_t)state->d); break;
		case 0xa3: and(state, (uint16_t)state->e); break;
		case 0xa4: and(state, (uint16_t)state->h); break;
		case 0xa5: and(state, (uint16_t)state->l); break;
		case 0xa6:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            and(state, (uint16_t)state->mem[addr]);
            break;
        }
		case 0xa7: and(state, (uint16_t)state->a); break;
		
        // XRA
        case 0xa8: xor(state, (uint16_t)state->b); break;
		case 0xa9: xor(state, (uint16_t)state->c); break;
		case 0xaa: xor(state, (uint16_t)state->d); break;
		case 0xab: xor(state, (uint16_t)state->e); break;
		case 0xac: xor(state, (uint16_t)state->h); break;
		case 0xad: xor(state, (uint16_t)state->l); break;
		case 0xae:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            xor(state, (uint16_t)state->mem[addr]);
            break;
        }
		case 0xaf: xor(state, (uint16_t)state->a); break;

        // ORA
		case 0xb0: or(state, (uint16_t)state->b); break;
		case 0xb1: or(state, (uint16_t)state->c); break;
		case 0xb2: or(state, (uint16_t)state->d); break;
		case 0xb3: or(state, (uint16_t)state->e); break;
		case 0xb4: or(state, (uint16_t)state->h); break;
		case 0xb5: or(state, (uint16_t)state->l); break;
		case 0xb6:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            or(state, (uint16_t)state->mem[addr]);
            break;
        }
		case 0xb7: or(state, (uint16_t)state->a); break;
		
        // CMP
        case 0xb8: cmp(state, (uint16_t)state->b); break;
		case 0xb9: cmp(state, (uint16_t)state->c); break;
		case 0xba: cmp(state, (uint16_t)state->d); break;
		case 0xbb: cmp(state, (uint16_t)state->e); break;
		case 0xbc: cmp(state, (uint16_t)state->h); break;
		case 0xbd: cmp(state, (uint16_t)state->l); break;
		case 0xbe:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            cmp(state, (uint16_t)state->mem[addr]);
            break;
        }
		case 0xbf: cmp(state, (uint16_t)state->a); break;

        // INX
        case 0x03: inx(&(state->b), &(state->c)); break;
        case 0x13: inx(&(state->d), &(state->e)); break;
        case 0x23: inx(&(state->h), &(state->l)); break;
        case 0x33: state->sp++; break;

        // DCX
        case 0x0B: dcx(&(state->b), &(state->c)); break;
        case 0x1B: dcx(&(state->d), &(state->e)); break;
        case 0x2B: dcx(&(state->h), &(state->l)); break;
        case 0x3B: state->sp--; break;

        // DAD
        case 0x09:
        {
            dad(state, (uint16_t)(state->b) << 8 | (uint16_t)(state->c));
            break;
        }
        case 0x19:
        {
            dad(state, (uint16_t)(state->d) << 8 | (uint16_t)(state->e));
            break;
        }
        case 0x29:
        {
            dad(state, (uint16_t)(state->h) << 8 | (uint16_t)(state->l));
            break;
        }
        case 0x39: dad(state, state->sp); break;
        
        // INR
        case 0x04: inr(state, &(state->b)); break;
        case 0x0C: inr(state, &(state->c)); break;
        case 0x14: inr(state, &(state->d)); break;
        case 0x1C: inr(state, &(state->e)); break;
        case 0x24: inr(state, &(state->h)); break;
        case 0x2C: inr(state, &(state->l)); break;
        case 0x34:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            inr(state, &(state->mem[addr]));
            break;
        }
        case 0x3C: inr(state, &(state->a)); break;

        // DCR
        case 0x05: dcr(state, &(state->b)); break;
        case 0x0D: dcr(state, &(state->c)); break;
        case 0x15: dcr(state, &(state->d)); break;
        case 0x1D: dcr(state, &(state->e)); break;
        case 0x25: dcr(state, &(state->h)); break;
        case 0x2D: dcr(state, &(state->l)); break;
        case 0x35:
        {
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            dcr(state, &(state->mem[addr]));
            break;
        }
        case 0x3D: dcr(state, &(state->a)); break;

        /* 2 byte codes */

        // for now just skip over data
        case 0xD3: state->pc++; break; // printf("OUT    #$%02x", code[1]); break;
        case 0xDB: state->pc++; break; // printf("IN    #$%02x", code[1]); break;
        
        // CPI
        case 0xFE:
        {
            state->pc++;
            cmp(state, (uint16_t)code[1]);
            break;
        }

        // XRI
        case 0xEE:
        {
            state->pc++;
            xor(state, (uint16_t)code[1]);
            break;
        }

        // ORI
        case 0xF6:
        {
            state->pc++;
            or(state, (uint16_t)code[1]);
            break;
        }

        // ANI
        case 0xE6:
        {
            state->pc++;
            and(state, (uint16_t)code[1]);
            break;
        }

        //SBI
        case 0xDE:
        {
            state->pc++;
            sub(state, (uint16_t)code[1], true);
            break;
        }

        // SUI
        case 0xD6:
        {
            state->pc++;
            sub(state, (uint16_t)code[1], false);
            break;
        }

        // ACI
        case 0xCE:
        {
            state->pc++;
            add(state, (uint16_t)code[1], true);
            break;
        }

        // ADI
        case 0xC6:
        {
            state->pc++;
            sub(state, (uint16_t)code[1], false);
            break;
        }

        // MVI
        case 0x06: 
        {
            state->pc++;
            mov(&(state->b), &(code[1]));
            break;
        }
        case 0x0E:
        {
            state->pc++;
            mov(&(state->c), &(code[1]));
            break;
        }
        case 0x16:
        {
            state->pc++;
            mov(&(state->d), &(code[1]));
            break;
        }
        case 0x1E:
        {
            state->pc++;
            mov(&(state->e), &(code[1]));
            break;
        }
        case 0x26:
        {
            state->pc++;
            mov(&(state->h), &(code[1]));
            break;
        }
        case 0x2E:
        {
            state->pc++;
            mov(&(state->l), &(code[1]));
            break;
        }
        case 0x36:
        {
            state->pc++;
            uint16_t addr = ((uint16_t)state->h << 8) | (uint16_t)state->l;
            mov(&(state->mem[addr]), &(code[1]));
            break;
        }
        case 0x3E:
        {
            state->pc++;
            mov(&(state->a), &(code[1]));
            break;
        }

        /* 3 byte codes */

        // LDA
        case 0x3A:
        {
            state->pc += 2;
            state->a = state->mem[((uint16_t)(code[2]) << 8) | (uint16_t)code[1]];
            break;
        }

        // STA
        case 0x32:
        {
            state->pc += 2;
            state->mem[((uint16_t)(code[2]) << 8) | (uint16_t)code[1]] = state->a;
            break;
        }

        // SHLD
        case 0x22:
        {
            state->pc += 2;
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            state->mem[addr] = state->l;
            state->mem[addr + 1] = state->h;
            break;
        }

        // LHLD
        case 0x2A:
        {
            state->pc += 2;
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            state->l = state->mem[addr];
            state->h = state->mem[addr + 1];
            break;
        }

        // JMP
        case 0xC3:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, 0x1, addr);
            break;
        }

        // CALL
        case 0xCD:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, 0x1, addr);
            break;
        }

        // LXI
        case 0x01:
        {
            state->pc += 2;
            state->b = code[2];
            state->c = code[1];
            break;
        }
        case 0x11:
        {
            state->pc += 2;
            state->d = code[2];
            state->e = code[1];
            break;
        }
        case 0x21:
        {
            state->pc += 2;
            state->h = code[2];
            state->l = code[1];
            break;
        }
        case 0x31:
        {
            state->pc += 2;
            state->sp = ((uint16_t)code[2] << 8) | (uint16_t)code[1];
            break;
        }

        /* Jccc */
        // JNZ
        case 0xC2:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, !(state->codes->z), addr);
            break;
        }
        // JZ
        case 0xCA:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, (state->codes->z), addr);
            break;
        }
        // JNC
        case 0xD2:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, !(state->codes->c), addr);
            break;
        }
        // JC
        case 0xDA:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, (state->codes->c), addr);
            break;
        }
        // JPO
        case 0xE2:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, !(state->codes->p), addr);
            break;
        }
        // JPE
        case 0xEA:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, (state->codes->p), addr);
            break;
        }
        // JP
        case 0xF2:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, !(state->codes->s), addr);
            break;
        }
        // JM
        case 0xFA:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            jump(state, (state->codes->s), addr);
            break;
        }

        /* Cccc */
        case 0xC4:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, !(state->codes->z), addr);
            break;
        }
        case 0xCC:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, (state->codes->z), addr);
            break;
        }
        case 0xD4:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, !(state->codes->c), addr);
            break;
        }
        case 0xDC:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, (state->codes->c), addr);
            break;
        }
        case 0xE4:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, !(state->codes->p), addr);
            break;
        }
        case 0xEC:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, (state->codes->p), addr);
            break;
        }
        case 0xF4:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, !(state->codes->s), addr);
            break;
        }
        case 0xFC:
        {
            uint16_t addr = ((uint16_t)(code[2]) << 8) | (uint16_t)code[1];
            call(state, (state->codes->s), addr);
            break;
        }
    }

    return 0;
}

State *init8080()
{
    State *state = calloc(1, sizeof(State));
    state->mem = malloc(0x10000); // 16kb
    if (state->mem == NULL)
        exit(1);
    state->codes = malloc(sizeof(Codes));
    state->a = 0;
    state->b = 0;
    state->c = 0;
    state->d = 0;
    state->e = 0;
    state->h = 0;
    state->l = 0;
    state->pc = 0;
    state->sp = 0xf000;
    state->codes->ac = 0;
    state->codes->c = 0;
    state->codes->p = 0;
    state->codes->s = 0;
    state->codes->z = 0;
    return state;
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

    int done = 0;
    while (done)
    {
        emulate8080(state);
        // done++;
    }
    free(state->codes);
    free(state);
    return 0;
}