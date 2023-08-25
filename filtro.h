#ifndef FILTRO_H
#define FILTRO_H

#include <stdbool.h> // SI HAGO LAS FUNCIONES PRIVADAS, INCLUIR ESTO EN EL .C
#include "imagen.h"

#define STR_INVERTIR "invertir"
#define STR_SATURAR "saturar"
#define STR_CAMBIAR_GAMA "gama"
#define STR_CAMBIAR_BRILLO "brillo"
#define STR_CAMBIAR_CONTRASTE "contraste"
#define STR_MEZCLAR "mezclar"
#define STR_SEPIA "sepia"
#define STR_MONOCROMO "monocromo"
#define STR_TOASTER "toaster"
#define STR_VALENCIA "valencia"

enum filtro_enum {
    FILTRO_INVERTIR,
    FILTRO_SATURAR,
    FILTRO_CAMBIAR_GAMA,
    FILTRO_CAMBIAR_BRILLO,
    FILTRO_CAMBIAR_CONTRASTE,
    FILTRO_MEZCLAR,
    FILTRO_SEPIA,
    FILTRO_MONOCROMO,
    FILTRO_TOASTER,
    FILTRO_VALENCIA
};

typedef enum filtro_enum filtro_enum_t;

typedef pixel_t (*filtro_funcion_t)(pixel_t, int);

typedef struct filtro_opcion filtro_opcion_t;


pixel_t filtro_invertir(pixel_t p, int _);
pixel_t filtro_saturar(pixel_t p, int porcentaje);
pixel_t filtro_cambiar_gama(pixel_t p, int desplazamiento);
pixel_t filtro_cambiar_brillo(pixel_t p, int porcentaje);
pixel_t filtro_cambiar_contraste(pixel_t p, int porcentaje);
pixel_t filtro_mezclar(pixel_t p, int color);
pixel_t filtro_sepia(pixel_t p, int _);
pixel_t filtro_monocromo(pixel_t p, int porcentaje_umbral);
pixel_t filtro_toaster(pixel_t p, int _);
pixel_t filtro_valencia(pixel_t p, int _);

bool cadena_a_filtro(const char *nombre, filtro_enum_t *filtro);
int numero_de_parametros(filtro_enum_t filtro);

imagen_t *aplicar_filtro(const imagen_t *imagen, filtro_enum_t filtro, int parametro);

#endif
