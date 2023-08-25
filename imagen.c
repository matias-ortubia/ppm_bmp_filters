#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "pixel.h"
#include "imagen.h"
#include "endianness.h"

#define MAX_DIGITS 20

struct imagen {
    pixel_t **lienzo;
    size_t ancho, alto;
};


static int leer_entero(FILE *f)
{
    size_t i = 0;
    int c;
    char num[MAX_DIGITS];

    c = fgetc(f);
    while(i < MAX_DIGITS - 1)
    {
        if(c == '#')
        {
            while(c != '\n')
                c = fgetc(f);

            c = fgetc(f);

            continue;
        }

        if(isspace(c))
        {
            if(isdigit(num[i - 1]))
                break;

            c = fgetc(f);
            continue;
        }

        if(c == EOF)
            return c;

        num[i] = c;

        c = fgetc(f);
        i++;
    }

    num[i] = '\0';

    return atoi(num);
}

static bool validar_encabezado_PPM(FILE *f)
{
    int c;
    while((c = fgetc(f)) != EOF)
    {
        if(c == 'P')
        {
            c = fgetc(f);
            if(c == '3')
            {
                return true;
            }
        }
    }
    return false;
}

static bool validar_encabezado_BMP(FILE *f)
{
    char str[3];

    fread(str, sizeof(char), 2, f);
    str[2] = '\0';
    
    return strcmp(str, "BM") == 0 ? true : false;
}

imagen_t *imagen_crear(size_t ancho, size_t alto)
{
    imagen_t *imagen;
    pixel_t **lienzo;
    size_t i;

    imagen = malloc(sizeof(imagen_t));
    if(imagen == NULL)
        return NULL;

    imagen->ancho = ancho;
    imagen->alto = alto;

    lienzo = malloc(alto * sizeof(pixel_t *));
    if(lienzo == NULL)
    {
        free(imagen);
        return NULL;
    }

    for(i = 0; i < alto; i++)
    {
        lienzo[i] = malloc(ancho * sizeof(pixel_t));
        if(lienzo[i] == NULL)
        {
            imagen->alto = i;
            imagen_destruir(imagen);
            return NULL;
        }
    }
    imagen->lienzo = lienzo;

    return imagen;
}

imagen_t *imagen_leer_PPM(FILE *f)
{
    imagen_t *imagen;
    size_t ancho, alto, max;
    size_t x, y;

    if(validar_encabezado_PPM(f) == false)
    {
        return NULL;
    }

    ancho = leer_entero(f);
    alto = leer_entero(f);
    max = leer_entero(f);

    if(max != 255)
        return NULL;

    imagen = imagen_crear(ancho, alto);
    if(imagen == NULL)
    {
        return NULL;
    }

    for(y = 0; y < alto; y++)
        for(x = 0; x < ancho; x++) {
            componente_t rojo = leer_entero(f);
            componente_t verde = leer_entero(f);
            componente_t azul = leer_entero(f);
            imagen->lienzo[y][x] = pixel_desde_rgb(rojo, verde, azul);
        }

    return imagen;
}

imagen_t *imagen_leer_BMP(FILE *f)
{
    imagen_t *imagen;
    size_t ancho, alto;
    size_t aux[28];

    if(validar_encabezado_BMP(f) == false)
        return NULL;

    fread(aux, 1, 16, f); // Se saltea el resto del encabezado de BMP y el tamano, ya que no se va a usar

    ancho = leer_int32_little_endian(f);
    alto = leer_int32_little_endian(f);

    imagen = imagen_crear(ancho, alto);
    if(imagen == NULL)
        return NULL;

    fread(aux, 1, 28, f); //Se saltea el resto del encabezado de Imagen de BMP

    for(size_t y = 0; y < imagen->alto; y++)
    {
        for(size_t x = 0; x < imagen->ancho; x++)
        {
            componente_t azul;
            fread(&azul, sizeof(componente_t), 1, f);

            componente_t verde;
            fread(&verde, sizeof(componente_t), 1, f);

            componente_t rojo;
            fread(&rojo, sizeof(componente_t), 1, f);

            imagen->lienzo[imagen->alto - 1 - y][x] = pixel_desde_rgb(rojo, verde, azul);
        }
        /* Si el ancho no es un multiplo de 4, hay 0 agregados por el scanline, por lo que hay que saltear esos bytes. */
        if(imagen->ancho % 4)
        {
            fseek(f, imagen->ancho % 4, SEEK_CUR);
        }
    }
    return imagen;
}

void imagen_destruir(imagen_t *imagen)
{
    size_t i;
    size_t n = imagen->alto;

    for(i = 0; i < n; i++)
    {
        free(imagen->lienzo[i]);
    }
    free(imagen->lienzo);
    free(imagen);
}

void imagen_escribir_PPM(const imagen_t *imagen, FILE *f)
{
    size_t ancho = imagen->ancho;
    size_t alto = imagen->alto;

    fprintf(f, "P3\n");
    fprintf(f, "%zd %zd 255\n", ancho, alto);

    for(int y = 0; y < alto; y++)
        for(int x = 0; x < ancho; x++) {
            pixel_t pixel = imagen->lienzo[y][x];
            fprintf(f, "%d %d %d\n", pixel_get_rojo(pixel), pixel_get_verde(pixel), pixel_get_azul(pixel));
        }
}

void imagen_escribir_BMP(const imagen_t *imagen, FILE *f)
{
    /* Escritura del encabezado de archivo */
    /* Tipo */
    fwrite("BM", sizeof(char), 2, f);
    /* Tamano del archivo */
    int32_t scanline = imagen->ancho * 3 + (4 - (imagen->ancho * 3) % 4);
    int32_t tam = 54 + imagen->alto * scanline;
    escribir_int32_little_endian(f, tam);
    /* Reservado */
    escribir_int16_little_endian(f, 0);
    /* Reservado */
    escribir_int16_little_endian(f, 0);
    /* Offset */
    escribir_int32_little_endian(f, 54);

    /* Escritura del encabezado de imagen */
    /* Tamano */
    escribir_int32_little_endian(f, 40);
    /* Ancho */
    escribir_int32_little_endian(f, imagen->ancho);
    /* Alto */
    escribir_int32_little_endian(f, imagen->alto);
    /* Planos */
    escribir_int16_little_endian(f, 1);
    /* Bits de color */
    escribir_int16_little_endian(f, 24);
    /* Compresion */
    escribir_int32_little_endian(f, 0);
    /* Tamano de imagen */
    escribir_int32_little_endian(f, 0);
    /* Resolucion X */
    escribir_int32_little_endian(f, 0);
    /* Resolucion Y */
    escribir_int32_little_endian(f, 0);
    /* Tablas de color */
    escribir_int32_little_endian(f, 0);
    /* Colores importantes */
    escribir_int32_little_endian(f, 0);
    /* Pixeles */
    int x, y;
    componente_t r, g, b;
    for(y = imagen->alto - 1; y >= 0; y--)
    {
        for(x = 0; x < imagen->ancho; x++)
        {
            b = pixel_get_azul(imagen->lienzo[y][x]);
            fwrite(&b, sizeof(componente_t), 1, f);
            g = pixel_get_verde(imagen->lienzo[y][x]);   
            fwrite(&g, sizeof(componente_t), 1, f);
            r = pixel_get_rojo(imagen->lienzo[y][x]);
            fwrite(&r, sizeof(componente_t), 1, f);
        }
        /* Si el ancho no es un multiplo de 4, se agregan 0 para completar el scanline. */
        if(imagen->ancho % 4)
        {
            for(x *= 3; x < scanline; x++)
            {
                uint8_t cero = 0;
                fwrite(&cero, sizeof(int8_t), 1, f);
            }
        }
    }
}

imagen_t *imagen_recortar(const imagen_t *imagen, size_t x0, size_t y0, size_t ancho, size_t alto)
{
    imagen_t *recorte;
    size_t x, y;
    
    if((x0 >= imagen->ancho) || (y0 >= imagen->alto))
    {
        return NULL;
    }

    if(x0 + ancho > imagen->ancho)
    {
        ancho = imagen->ancho - x0;
    }

    if(y0 + alto > imagen->alto)
    {
        alto = imagen->alto - y0;
    }

    recorte = imagen_crear(ancho, alto);
    if(recorte == NULL)
    {
        return NULL;
    }

    for(y = 0; y < alto; y++)
    {
        for(x = 0; x < ancho; x++)
        {
            recorte->lienzo[y][x] = imagen->lienzo[y + y0][x + x0];
        }
    }

    return recorte;
}

imagen_t *imagen_clonar(const imagen_t *imagen)
{
    return imagen_recortar(imagen, 0, 0, imagen->ancho, imagen->alto);
}

imagen_t *imagen_espejar_horizontalmente(const imagen_t *imagen)
{
    imagen_t *imagen_espejada;

    imagen_espejada = imagen_crear(imagen->ancho, imagen->alto);
    if(imagen_espejada == NULL)
        return NULL;

    for(size_t y = 0; y < imagen->alto; y++)
        for(size_t x = 0; x < imagen->ancho; x++)
            imagen_espejada->lienzo[y][x] = imagen->lienzo[y][imagen->ancho - x - 1];

    return imagen_espejada;
}

imagen_t *imagen_espejar_verticalmente(const imagen_t *imagen)
{
    imagen_t *imagen_espejada;

    imagen_espejada = imagen_crear(imagen->ancho, imagen->alto);
    if(imagen_espejada == NULL)
        return NULL;

    for(size_t y = 0; y < imagen->alto; y++)
        for(size_t x = 0; x < imagen->ancho; x++)
            imagen_espejada->lienzo[y][x] = imagen->lienzo[imagen->alto - y - 1][x];

    return imagen_espejada;
}

size_t imagen_obtener_ancho(const imagen_t *imagen)
{
    return imagen->ancho;
}

size_t imagen_obtener_alto(const imagen_t *imagen)
{
    return imagen->alto;
}

pixel_t imagen_obtener_pixel(const imagen_t *imagen, size_t fil, size_t col)
{
    return imagen->lienzo[fil][col];
}

pixel_t **imagen_obtener_lienzo(const imagen_t *imagen)
{
    return imagen->lienzo;
}
