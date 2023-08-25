#include <stdio.h>
#include <stdint.h>
#include "endianness.h"

#define SHIFT_BYTE 8

void escribir_int16_little_endian(FILE *f, int16_t v)
{
    uint8_t a, b;

    a = v;
    b = v >> 8;
    
    fwrite(&a, sizeof(uint8_t), 1, f);
    fwrite(&b, sizeof(uint8_t), 1, f);
}

void escribir_int32_little_endian(FILE *f, int32_t v)
{
    uint8_t a, b, c, d;

    a = v;
    b = v >> 8;
    c = v >> 16;
    d = v >> 24;

    fwrite(&a, sizeof(uint8_t), 1, f);
    fwrite(&b, sizeof(uint8_t), 1, f);
    fwrite(&c, sizeof(uint8_t), 1, f);
    fwrite(&d, sizeof(uint8_t), 1, f);
}

uint16_t leer_int16_little_endian(FILE *f)
{
    uint16_t numero = 0;
    uint8_t a, b;

    fread(&a, 1, 1, f);
    fread(&b, 1, 1, f);

    return (((numero | b) << SHIFT_BYTE) | a);
}

uint32_t leer_int32_little_endian(FILE *f)
{
    uint32_t numero = 0;
    uint8_t a, b, c, d;
    
    fread(&a, 1, 1, f);
    fread(&b, 1, 1, f);
    fread(&c, 1, 1, f);
    fread(&d, 1, 1, f);

    return (((((((numero | d) << SHIFT_BYTE) | c) << SHIFT_BYTE) | b) << SHIFT_BYTE) | a);
}
