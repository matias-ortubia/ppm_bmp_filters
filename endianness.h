#ifndef ENDIANNESS_H
#define ENDIANNESS_H

void escribir_int16_little_endian(FILE *f, int16_t v);
void escribir_int32_little_endian(FILE *f, int32_t v);

uint16_t leer_int16_little_endian(FILE *f);
uint32_t leer_int32_little_endian(FILE *f);

#endif
