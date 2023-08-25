#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "argumentos.h"
#include "pixel.h"
#include "imagen.h"
#include "filtro.h"

#define BANDERAS_CANTIDAD 6

struct arg_nodo
{
    char *bandera; // Bandera ingresada 
    char *nombre; // Puede ser nombre de archivo o de filtro
    int filtro_n; // El parametro que podria estar despues del nombre de un filtro
    filtro_enum_t filtro; // El tipo enumerativo que representa al filtro elegido
    int w; // Ancho del recorte
    int h; // Alto del recorte
    int x; // Parametro X0 para recorte
    int y; // Parametro Y0 para recorte
};

struct argumentos 
{
    struct arg_nodo **args; // Es un arreglo dinamico de punteros a arg_nodo, con cantidad de nodos igual a n_args
    size_t n_args; // Es la cantidad de arg_nodo en el arreglo dinamico de punteros args
};

typedef enum {
    BANDERA_INPUT,
    BANDERA_OUTPUT,
    BANDERA_FILTER,
    BANDERA_CROP,
    BANDERA_HORIZONTAL,
    BANDERA_VERTICAL
} banderas_t;

char *banderas_str[BANDERAS_CANTIDAD] = {
    [BANDERA_INPUT] = BANDERA_STR_INPUT,
    [BANDERA_OUTPUT] = BANDERA_STR_OUTPUT,
    [BANDERA_FILTER] = BANDERA_STR_FILTER,
    [BANDERA_CROP] = BANDERA_STR_CROP,
    [BANDERA_HORIZONTAL] = BANDERA_STR_HORIZONTAL,
    [BANDERA_VERTICAL] = BANDERA_STR_VERTICAL
};


static argumentos_t *_argumentos_crear(void)
{
    argumentos_t *nuevo = malloc(sizeof(argumentos_t));
    if(nuevo == NULL)
        return NULL;

    nuevo->args = malloc(sizeof(struct arg_nodo *));
    if(nuevo->args == NULL)
    {
        free(nuevo);
        return NULL;
    }
    nuevo->n_args = 0;

    return nuevo;
}

static struct arg_nodo *arg_nodo_crear(void)
{
    struct arg_nodo *arg = malloc(sizeof(struct arg_nodo));
    if(arg == NULL)
        return NULL;

    arg->bandera = NULL;
    arg->nombre = NULL;

    return arg;
}

static bool _argumentos_agregar_final(argumentos_t *argumentos)
{
    struct arg_nodo **aux = realloc(argumentos->args, sizeof(struct arg_nodo *) * (argumentos->n_args + 1));
    if(aux == NULL)
        return false;
    
    argumentos->args = aux;

    argumentos->args[argumentos->n_args] = arg_nodo_crear();
    if(argumentos->args[argumentos->n_args] == NULL)
        return false;

    argumentos->n_args++; // Se mantiene el invariante de representacion de argumentos_t 

    return true;
}

void argumentos_destruir(argumentos_t *argumentos)
{
    for(size_t i = 0; i < argumentos->n_args; i++)
    {
        free(argumentos->args[i]->bandera);
        free(argumentos->args[i]->nombre);
        free(argumentos->args[i]);
    }
    free(argumentos->args);
    free(argumentos);
}

static bool _argumentos_validar_bandera(const char *bandera)
{
    for(size_t i = 0; i < BANDERAS_CANTIDAD; i++)
    {
        if(strcmp(bandera, banderas_str[i]) == 0)
            return true;
    }
    return false;
}

argumentos_t *argumentos_leer(int argc, char *argv[])
{
    argumentos_t *argumentos = _argumentos_crear();
    if(argumentos == NULL)
        return NULL;

    size_t i = 1;
    size_t j = 0;
    while(i < argc)
    {
        if(argv[i][0] == '-')
        {
            if(_argumentos_agregar_final(argumentos) == false)
            {
                argumentos_destruir(argumentos);
                return NULL;
            }

            argumentos->args[j]->bandera = malloc(strlen(argv[i]) + 1);
            if(argumentos->args[j]->bandera == NULL)
            {
                argumentos_destruir(argumentos);
                return NULL;
            }

            strcpy(argumentos->args[j]->bandera, argv[i]);
            i++;

            if(_argumentos_validar_bandera(argumentos->args[j]->bandera) == false) // Valida la bandera ingresada.
            {
                argumentos_destruir(argumentos);
                return NULL;
            }

            if(strcmp(argumentos->args[j]->bandera, banderas_str[BANDERA_INPUT]) == 0 || strcmp(argumentos->args[j]->bandera, banderas_str[BANDERA_OUTPUT]) == 0)
            {
                if(i == argc || argv[i][0] == '-') // Si el siguiente parametro es otra bandera, significa que no se ingreso un nombre de archivo.
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }

                argumentos->args[j]->nombre = malloc(strlen(argv[i]) + 1);
                if(argumentos->args[j]->nombre == NULL)
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }
                strcpy(argumentos->args[j]->nombre, argv[i]);

                i++;
            }

            if(strcmp(argumentos->args[j]->bandera, banderas_str[BANDERA_CROP]) == 0)
            {
                if(i == argc || argv[i][0] == '-') // No se especifico el area de recorte.
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }

                char *endptr;
                
                // Se carga el ancho del recorte
                argumentos->args[j]->w = strtol(argv[i], &endptr, 10);
                if(*endptr != 'x')
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }
                // Se carga el alto del recorte
                argumentos->args[j]->h = strtol(endptr + 1, &endptr, 10);
                if(*endptr != '+')
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }
                // Se carga el parametro X del recorte
                argumentos->args[j]->x = strtol(endptr + 1, &endptr, 10);
                if(*endptr != '+')
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }

                // Se carga el parametro Y del recorte
                argumentos->args[j]->y = strtol(endptr + 1, &endptr, 10);
                if(*endptr != '\0') // Si lo ultimo no es un '\0', se ingreso algo que no era un numero al final y se considera invalido.
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }
                i++;

            }

            if(strcmp(argumentos->args[j]->bandera, banderas_str[BANDERA_FILTER]) == 0)
            {
                if(i == argc || argv[i][0] == '-') // No se especifico el filtro.
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }

                argumentos->args[j]->nombre = malloc(strlen(argv[i]) + 1);
                if(argumentos->args[j]->nombre == NULL)
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }

                strcpy(argumentos->args[j]->nombre, argv[i]);
                i++;

                argumentos->args[j]->filtro = 0;
                if(cadena_a_filtro(argumentos->args[j]->nombre, &argumentos->args[j]->filtro) == false)
                {
                    argumentos_destruir(argumentos);
                    return NULL;
                }
                  
                if(i < argc && argv[i][0] != '-')
                {
                    if(strcmp(argumentos->args[j]->nombre, STR_MEZCLAR) == 0)
                    {
                        argumentos->args[j]->filtro_n = pixel_desde_nombre(argv[i]);
                    }
                    else
                    {
                        char *endptr;
                        argumentos->args[j]->filtro_n = strtol(argv[i], &endptr, 10);
                        if(*endptr != '\0') // Se ingreso algo que no es un numero.
                        {
                            argumentos_destruir(argumentos);
                            return NULL;
                        }
                    }
                    i++;
                }
            } 
            
            j++;
        }
        else
        {
            argumentos_destruir(argumentos);
            return NULL;
        }
    }
    return argumentos;
}

size_t argumentos_obtener_cantidad(argumentos_t *argumentos)
{
    return argumentos->n_args;
}

char *argumentos_obtener_bandera(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->bandera;
}

char *argumentos_obtener_nombre(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->nombre;
}

int argumentos_obtener_filtro(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->filtro;
}

int argumentos_obtener_ancho_recorte(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->w;
}

int argumentos_obtener_alto_recorte(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->h;
}

int argumentos_obtener_X_recorte(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->x;
}

int argumentos_obtener_Y_recorte(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->y;
}

int argumentos_obtener_parametro_filtro(argumentos_t *argumentos, size_t n)
{
    return argumentos->args[n]->filtro_n;
}
