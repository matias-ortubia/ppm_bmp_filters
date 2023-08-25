#include <stdlib.h>
#include <string.h>
#include "procesamiento.h"

static bool _tiene_extension(char *nombre, const char *extension)
{
    char *ext;

    size_t i;
    for(i = 0; nombre[i] != '.' && nombre[i] != '\0'; i++);

    ext = malloc(strlen(nombre) - i);
    if(ext == NULL)
        return false;

    size_t j;
    for(i++, j = 0; nombre[i] != '\0'; i++, j++)
        ext[j] = nombre[i];
    ext[j] = '\0';

    if(strcmp(ext, extension) == 0)
    {
        free(ext);
        return true;
    }

    free(ext);
    return false;
}

bool es_ppm(char *nombre)
{
    return _tiene_extension(nombre, "ppm");
}

bool es_bmp(char *nombre)
{
    return _tiene_extension(nombre, "bmp");
}
