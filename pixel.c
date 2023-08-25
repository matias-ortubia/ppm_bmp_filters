#include <string.h>
#include <stdint.h>
#include <math.h>
#include "pixel.h"

#define SHIFT_COLOR 8
#define SHIFT_ROJO 16
#define SHIFT_VERDE 8
//#define SHIFT_AZUL 0
#define MASK_ROJO 0xFF0000
#define MASK_VERDE 0x00FF00
#define MASK_AZUL 0x0000FF
#define INIT_COLOR 0
#define CANT_COLORES 16
#define COLOR_DEFAULT 0x000000 // Color negro

#define MAX_NOMBRE 10

typedef struct {
    componente_t r, g, b;
    char nombre[MAX_NOMBRE];
} color_t;

typedef enum {
    COLOR_BLACK, 
    COLOR_WHITE, 
    COLOR_RED, 
    COLOR_LIME, 
    COLOR_BLUE, 
    COLOR_YELLOW, 
    COLOR_CYAN, 
    COLOR_MAGENTA, 
    COLOR_SILVER, 
    COLOR_GRAY, 
    COLOR_MAROON, 
    COLOR_OLIVE, 
    COLOR_GREEN, 
    COLOR_PURPLE, 
    COLOR_TEAL, 
    COLOR_NAVY
} nombre_t;

const color_t color[] = {
    [COLOR_BLACK] = {0, 0, 0, STR_BLACK}, 
    [COLOR_WHITE] = {255, 255, 255, STR_WHITE}, 
    [COLOR_RED] = {255, 0, 0, STR_RED}, 
    [COLOR_LIME] = {0, 255, 0, STR_LIME}, 
    [COLOR_BLUE] = {0, 0, 255, STR_BLUE}, 
    [COLOR_YELLOW] = {255, 255, 0, STR_YELLOW}, 
    [COLOR_CYAN] = {0, 255, 255, STR_CYAN}, 
    [COLOR_MAGENTA] = {255, 0, 255, STR_MAGENTA}, 
    [COLOR_SILVER] = {192, 192, 192, STR_SILVER}, 
    [COLOR_GRAY] = {128, 128, 128, STR_GRAY},
    [COLOR_MAROON] = {128, 0, 0, STR_MAROON}, 
    [COLOR_OLIVE] = {128, 128, 0, STR_OLIVE}, 
    [COLOR_GREEN] = {0, 128, 0, STR_GREEN}, 
    [COLOR_PURPLE] = {128, 0, 128, STR_PURPLE}, 
    [COLOR_TEAL] = {0, 128, 128, STR_TEAL}, 
    [COLOR_NAVY] = {0, 0, 128, STR_NAVY}
};

/* Devuelve un color representado en pixel_t a partir de sus componentes RGB */
pixel_t pixel_desde_rgb(componente_t rojo, componente_t verde, componente_t azul)
{ 
    return (((((INIT_COLOR | rojo) << SHIFT_COLOR) | verde ) << SHIFT_COLOR ) | azul);
}

pixel_t pixel_desde_hsv(short h, float s, float v) {
    float c = s * v;
    float x = c * (1 - fabs(fmodf(h / 60.0, 2) - 1));
    float m = v - c;

    float r = 0, v_ = 0, a = 0;

    if(h < 60)
        r = c, v_ = x;
    else if(h < 120)
        v_ = c, r = x;
    else if(h < 180)
        v_ = c, a = x;
    else if(h < 240)
        a = c, v_ = x;
    else if(h < 300)
        a = c, r = x;
    else
        r = c, a = x;

    return pixel_desde_rgb((r + m) * 255, (v_ + m) * 255, (a + m) * 255);
}

/* Recibe un pixel. Devuelve su componente roja. */
componente_t pixel_get_rojo(pixel_t p)
{
    return (p & MASK_ROJO) >> SHIFT_ROJO;
}

/* Recibe un pixel. Devuelve su componente verde. */
componente_t pixel_get_verde(pixel_t p)
{
    return (p & MASK_VERDE) >> SHIFT_VERDE;
}

/* Recibe un pixel. Devuelve su componente azul. */
componente_t pixel_get_azul(pixel_t p)
{
    return p & MASK_AZUL; // >> SHIFT_AZUL;
}

void pixel_a_hsv(pixel_t p, short *h, float *s, float *v) {
    float rp = pixel_get_rojo(p) / 255.0;
    float vp = pixel_get_verde(p) / 255.0;
    float ap = pixel_get_azul(p) / 255.0;

    float cmax = rp > vp ? rp : vp;
    cmax = cmax > ap ? cmax : ap;

    float cmin = rp < vp ? rp : vp;
    cmin = cmin < ap ? cmin : ap;

    float delta = cmax - cmin;

    if(delta == 0)
        *h = 0;
    else if(cmax == rp)
        *h = (int)(60 * (vp - ap) / delta + 360.5) % 360;
    else if(cmax == vp)
        *h = (int)(60 * (ap - rp) / delta + 120.5) % 360;
    else
        *h = (int)(60 * (rp - vp) / delta + 240.5) % 360;

    *s = cmax == 0 ? 0 : (delta / cmax);
    *v = cmax;
}

/* Recibe el nombre de un color, y devuelve un pixel con ese color.
 * Si el color indicado no corresponde, devuelve negro. */
pixel_t pixel_desde_nombre(const char *nombre)
{
    size_t i;
    for(i = 0; i < CANT_COLORES; i++)
    {
        if(!strcmp(color[i].nombre, nombre))
        {
            return pixel_desde_rgb(color[i].r, color[i].g, color[i].b);
        }
    }

    return COLOR_DEFAULT;
}
