#include <stdio.h>
#include <string.h>
#include "pixel.h"
#include "filtro.h"

#define MAX_FILTRO_NOMBRE 20
#define CANT_FILTROS 10

struct filtro_opcion{
    int parametros;
    filtro_funcion_t funcion;
    filtro_enum_t filtro;
    char nombre[MAX_FILTRO_NOMBRE];
}; 

const filtro_opcion_t filtro_opcion[] = {
    {0, filtro_invertir, FILTRO_INVERTIR, STR_INVERTIR},
    {1, filtro_saturar, FILTRO_SATURAR, STR_SATURAR},
    {1, filtro_cambiar_gama, FILTRO_CAMBIAR_GAMA, STR_CAMBIAR_GAMA},
    {1, filtro_cambiar_brillo, FILTRO_CAMBIAR_BRILLO, STR_CAMBIAR_BRILLO},
    {1, filtro_cambiar_contraste, FILTRO_CAMBIAR_CONTRASTE, STR_CAMBIAR_CONTRASTE},
    {1, filtro_mezclar, FILTRO_MEZCLAR, STR_MEZCLAR},
    {0, filtro_sepia, FILTRO_SEPIA, STR_SEPIA},
    {1, filtro_monocromo, FILTRO_MONOCROMO, STR_MONOCROMO},
    {0, filtro_toaster, FILTRO_TOASTER, STR_TOASTER},
    {0, filtro_valencia, FILTRO_VALENCIA, STR_VALENCIA}
};


/* Recibe el nombre de un filtro y un puntero al filtro a devolver.
 * Devuelve true si pudo convertir la cadena en etiqueta, o false en caso contrario. */
bool cadena_a_filtro(const char *nombre, filtro_enum_t *filtro)
{
    size_t i;
    for(i = 0; i < CANT_FILTROS; i++)
    {
        if(!strcmp(nombre, filtro_opcion[i].nombre))
        {
            *filtro = filtro_opcion[i].filtro;
            return true;
        }
    }

    return false;
}

/* Recibe una etiqueta del enumerativo en filtro.
 * Devuelve el numero de parametros que requiere ese filtro. */
int numero_de_parametros(filtro_enum_t filtro)
{
    return filtro_opcion[filtro].parametros;
}

pixel_t filtro_invertir(pixel_t p, int _) {
    return pixel_desde_rgb(255 - pixel_get_rojo(p), 255 - pixel_get_verde(p), 255 - pixel_get_azul(p));
}

pixel_t filtro_saturar(pixel_t p, int porcentaje) {
    short h;
    float s, v;
    pixel_a_hsv(p, &h, &s, &v);

    s *= (1 + porcentaje / 100.0);

    if(s < 0)
        s = 0;
    else if(s > 1)
        s = 1;

    return pixel_desde_hsv(h, s, v);
}

pixel_t filtro_cambiar_gama(pixel_t p, int desplazamiento) {
    short h;
    float s, v;
    pixel_a_hsv(p, &h, &s, &v);

    h = (h + desplazamiento) % 360;

    return pixel_desde_hsv(h, s, v);
}

pixel_t filtro_cambiar_brillo(pixel_t p, int porcentaje) {
    short h;
    float s, v;
    pixel_a_hsv(p, &h, &s, &v);

    v += porcentaje / 100.;

    if(v < 0)
        v = 0;
    else if(v > 1)
        v = 1;

    return pixel_desde_hsv(h, s, v);
}

pixel_t filtro_cambiar_contraste(pixel_t p, int porcentaje) {
    short h;
    float s, v;
    pixel_a_hsv(p, &h, &s, &v);

    v *= 1 + porcentaje / 100.;

    if(v < 0)
        v = 0;
    else if(v > 1)
        v = 1;

    return pixel_desde_hsv(h, s, v);
}

pixel_t filtro_mezclar(pixel_t p, int color) {
    pixel_t c = color;

    return pixel_desde_rgb(
        (pixel_get_rojo(p) + pixel_get_rojo(c)) / 2,
        (pixel_get_verde(p) + pixel_get_verde(c)) / 2,
        (pixel_get_azul(p) + pixel_get_azul(c)) / 2);
}

pixel_t filtro_sepia(pixel_t p, int _) {
    componente_t r = pixel_get_rojo(p);
    componente_t v = pixel_get_verde(p);
    componente_t a = pixel_get_azul(p);

    int rn = r * 0.393 + v * 0.769 + a * 0.189;
    int vn = r * 0.349 + v * 0.686 + a * 0.168;
    int an = r * 0.272 + v * 0.534 + a * 0.131;

    if(rn > 255) rn = 255;
    if(vn > 255) vn = 255;
    if(an > 255) an = 255;

    return pixel_desde_rgb(rn, vn, an);
}

pixel_t filtro_monocromo(pixel_t p, int porcentaje_umbral) {
    short h;
    float s, v;
    pixel_a_hsv(p, &h, &s, &v);

    return (v > porcentaje_umbral / 100.) ? pixel_desde_rgb(255, 255, 255) : pixel_desde_rgb(0, 0, 0);
}

pixel_t filtro_toaster(pixel_t p, int _) {
    p = filtro_mezclar(p, pixel_desde_rgb(51, 0, 0));

    short h;
    float s, v;
    pixel_a_hsv(p, &h, &s, &v);

    v = v * 1.5 + 0.2;
    s *= 0.8;
    h = (h + 20) % 360;

    if(v > 1)
        v = 1;

    return pixel_desde_hsv(h, s, v);
}

pixel_t filtro_valencia(pixel_t p, int _) {
    float r = pixel_get_rojo(p) / 100.;
    float ve = pixel_get_verde(p) / 100.;
    float a = pixel_get_azul(p) / 100.;

    r = 0.23 + r - 2 * 0.23 * r;
    ve = 0.01 + ve - 2 * 0.01 * ve;
    a = 0.22 + a - 2 * 0.22 * a;

    if(r > 1) r = 1;
    if(ve > 1) ve = 1;
    if(a > 1) a = 1;

    if(r < 0) r = 0;
    if(ve < 0) ve = 0;
    if(a < 0) a = 0;

    p = filtro_mezclar(p, pixel_desde_rgb(r * 255, ve * 255, a * 255));

    short h;
    float s, v;
    pixel_a_hsv(p, &h, &s, &v);

    v = v * 1.08 - 0.08;
    if(v > 1)
        v = 1;
    if(v < 0)
        v = 0;

    p = pixel_desde_hsv(h, s, v);
    pixel_t c = filtro_sepia(p, 0);

    return pixel_desde_rgb(
        pixel_get_rojo(p) * 0.92 + pixel_get_rojo(c) * 0.08,
        pixel_get_verde(p) * 0.92 + pixel_get_verde(c) * 0.08,
        pixel_get_azul(p) * 0.92 + pixel_get_azul(c) * 0.08);
}

imagen_t *aplicar_filtro(const imagen_t *imagen, filtro_enum_t filtro, int parametro)
{
    imagen_t *imagen_filtrada = imagen_clonar(imagen);
    if(imagen_filtrada == NULL)
        return NULL;

    pixel_t **lienzo_p;
    lienzo_p = imagen_obtener_lienzo(imagen_filtrada); // lienzo_p apunta al lienzo de la copia de la imagen, cargada en el heap

    for(size_t y = 0; y < imagen_obtener_alto(imagen); y++)
        for(size_t x = 0; x < imagen_obtener_ancho(imagen); x++)
            lienzo_p[y][x] = (filtro_opcion[filtro].funcion)(imagen_obtener_pixel(imagen, y, x), parametro);

    return imagen_filtrada;
}
