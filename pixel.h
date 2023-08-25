#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>

#define STR_BLACK "black"
#define STR_WHITE "white"
#define STR_RED "red"
#define STR_LIME "lime"
#define STR_BLUE "blue"
#define STR_YELLOW "yellow"
#define STR_CYAN "cyan"
#define STR_MAGENTA "magenta"
#define STR_SILVER "silver"
#define STR_GRAY "gray"
#define STR_MAROON "maroon"
#define STR_OLIVE "olive"
#define STR_GREEN "green"
#define STR_PURPLE "purple"
#define STR_TEAL "teal"
#define STR_NAVY "navy"

typedef uint8_t componente_t;
typedef uint32_t pixel_t;

pixel_t pixel_desde_rgb(componente_t rojo, componente_t verde, componente_t azul);
pixel_t pixel_desde_hsv(short h, float s, float v);
componente_t pixel_get_rojo(pixel_t p);
componente_t pixel_get_verde(pixel_t p);
componente_t pixel_get_azul(pixel_t p);
void pixel_a_hsv(pixel_t p, short *h, float *s, float *v);
pixel_t pixel_desde_nombre(const char *nombre);

#endif
