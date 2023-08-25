#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include <stdbool.h>

// Reciben una cadena de caracteres y devuelven true si la cadena termina con ".ppm" o ".bmp" respectivamente.
bool es_ppm(char *nombre);
bool es_bmp(char *nombre);

#endif
