#ifndef IMAGEN_H
#define IMAGEN_H

#include <stdio.h>
#include <stdbool.h> // BORRAR DESPUES DE SACAR VALIDAR BMP

typedef struct imagen imagen_t;

imagen_t *imagen_crear(size_t ancho, size_t alto);
void imagen_destruir(imagen_t *imagen);

imagen_t *imagen_leer_PPM(FILE *f);
imagen_t *imagen_leer_BMP(FILE *f);

void imagen_escribir_PPM(const imagen_t *imagen, FILE *f);
void imagen_escribir_BMP(const imagen_t *imagen, FILE *f);

imagen_t *imagen_recortar(const imagen_t *imagen, size_t x0, size_t y0, size_t ancho, size_t alto);
imagen_t *imagen_clonar(const imagen_t *imagen);

size_t imagen_obtener_ancho(const imagen_t *imagen);
size_t imagen_obtener_alto(const imagen_t *imagen);
pixel_t imagen_obtener_pixel(const imagen_t *imagen, size_t fil, size_t col);
pixel_t **imagen_obtener_lienzo(const imagen_t *imagen);

imagen_t *imagen_espejar_horizontalmente(const imagen_t *imagen);
imagen_t *imagen_espejar_verticalmente(const imagen_t *imagen);

#endif
