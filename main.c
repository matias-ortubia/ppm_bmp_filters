/**************************************************************************
 * Trabajo Practico 1 - Algoritmos y Programacion I - Curso 2 Essaya      * 
 * Nombre: Matias Emanuel Ortubia                                         *
 * Padron: 101572                                                         *
 * ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include "argumentos.h"
#include "procesamiento.h"
#include "pixel.h"
#include "imagen.h"
#include "filtro.h"


int main(int argc, char *argv[])
{
    // Lectura de argumentos
    argumentos_t *argumentos = argumentos_leer(argc, argv);
    if(argumentos == NULL)
    {
        fprintf(stderr, "Error.\n");
        return 1;
    }

    // Apertura del archivo de entrada y lectura de la imagen
    FILE *finput = NULL;
    imagen_t *imagen;
    size_t i;
    for(i = 0; i < argumentos_obtener_cantidad(argumentos) && finput == NULL; i++)
    {
        if(strcmp(argumentos_obtener_bandera(argumentos, i), BANDERA_STR_INPUT) == 0)
        {
            if(es_ppm(argumentos_obtener_nombre(argumentos, i)) == true)
            {
                finput = fopen(argumentos_obtener_nombre(argumentos, i), "rt");
                if(finput == NULL)
                {
                    fprintf(stderr, "No se pudo leer el archivo de entrada.\n");
                    argumentos_destruir(argumentos);
                    return 1;
                }
                imagen = imagen_leer_PPM(finput);
                if(imagen == NULL)
                {
                    fprintf(stderr, "No se pudo leer la imagen.\n");
                    argumentos_destruir(argumentos);
                    fclose(finput);
                    return 1;
                }
            }
            else 
            {
                if(es_bmp(argumentos_obtener_nombre(argumentos, i)) == true)
                {
                    finput = fopen(argumentos_obtener_nombre(argumentos, i), "rb");
                    if(finput == NULL)
                    {
                        fprintf(stderr, "No se pudo leer el archivo de entrada.\n");
                        argumentos_destruir(argumentos);
                        return 1;
                    }
                    imagen = imagen_leer_BMP(finput);
                    if(imagen == NULL)
                    {
                        fprintf(stderr, "No se pudo leer la imagen.\n");
                        argumentos_destruir(argumentos);
                        fclose(finput);
                        return 1;
                    }
                }
                else
                {
                    fprintf(stderr, "El archivo de entrada solo puede ser .ppm o .bmp\n");
                    argumentos_destruir(argumentos);
                    return 1;
                }
            }
        }
    }
    if(finput == NULL) // Si llego al final del ciclo y no encontro la bandera -input, la entrada es stdin.
    {
        finput = stdin;
        imagen = imagen_leer_PPM(stdin); // Primero se prueba si la imagen es ppm.
        if(imagen == NULL)
        {
            rewind(finput); // imagen_leer_PPM comenzo a leer la imagen, asi que es necesario rebobinar
            imagen = imagen_leer_BMP(stdin); // Se prueba si en vez de ppm era bmp.
            if(imagen == NULL)
            {
                fprintf(stderr, "No se pudo leer la imagen.\n");
                argumentos_destruir(argumentos);
                return 1; // stdin no se cierra
            }
        }
    }

    // Procesamiento de la imagen
    for(i = 0; i < argumentos_obtener_cantidad(argumentos); i++)
    {
        if(strcmp(argumentos_obtener_bandera(argumentos, i), BANDERA_STR_FILTER) == 0)
        {
            imagen_t *aux;
            filtro_enum_t filtro;
            if(cadena_a_filtro(argumentos_obtener_nombre(argumentos, i), &filtro) == false)
            {
                fprintf(stderr, "Filtro invalido.\n");
                argumentos_destruir(argumentos);
                if(finput != stdin) fclose(finput);
                imagen_destruir(imagen);
                return 1;
            }
            aux = aplicar_filtro(imagen, filtro, argumentos_obtener_parametro_filtro(argumentos, i));
            if(aux == NULL)
            {
                fprintf(stderr, "Error al aplicar el filtro.\n");
                argumentos_destruir(argumentos);
                if(finput != stdin) fclose(finput);
                imagen_destruir(imagen);
                return 1;
            }
            imagen_destruir(imagen);
            imagen = aux;
        }

        if(strcmp(argumentos_obtener_bandera(argumentos, i), BANDERA_STR_CROP) == 0)
        {
            imagen_t *aux;
            aux = imagen_recortar(imagen, argumentos_obtener_X_recorte(argumentos, i), argumentos_obtener_Y_recorte(argumentos, i), argumentos_obtener_ancho_recorte(argumentos, i), argumentos_obtener_alto_recorte(argumentos, i));
            if(aux == NULL)
            {
                fprintf(stderr, "Error al recortar la imagen.\n");
                argumentos_destruir(argumentos);
                if(finput != stdin) fclose(finput);
                imagen_destruir(imagen);
                return 1;
            }
            imagen_destruir(imagen);
            imagen = aux;
        }

        if(strcmp(argumentos_obtener_bandera(argumentos, i), BANDERA_STR_HORIZONTAL) == 0)
        {
            imagen_t *aux;
            aux = imagen_espejar_horizontalmente(imagen);
            if(aux == NULL)
            {
                fprintf(stderr, "Error al espejar horizontalmente.\n");
                argumentos_destruir(argumentos);
                if(finput != stdin) fclose(finput);
                imagen_destruir(imagen);
                return 1;
            }
            imagen_destruir(imagen);
            imagen = aux;
        }

        if(strcmp(argumentos_obtener_bandera(argumentos, i), BANDERA_STR_VERTICAL) == 0)
        {
            imagen_t *aux;
            aux = imagen_espejar_verticalmente(imagen);
            if(aux == NULL)
            {
                fprintf(stderr, "Error al espejar verticalmente.\n");
                argumentos_destruir(argumentos);
                if(finput != stdin) fclose(finput);
                imagen_destruir(imagen);
                return 1;
            }
            imagen_destruir(imagen);
            imagen = aux;
        }
    }

    // Apertura de archivo de salida
    FILE *foutput = NULL;
    for(i = 0; i < argumentos_obtener_cantidad(argumentos) && foutput == NULL; i++)
    {
        if(strcmp(argumentos_obtener_bandera(argumentos, i), BANDERA_STR_OUTPUT) == 0)
        {
            if(es_ppm(argumentos_obtener_nombre(argumentos, i)) == true)
            {
                foutput = fopen(argumentos_obtener_nombre(argumentos, i), "wt");
                if(foutput == NULL)
                {
                    fprintf(stderr, "No se pudo abrir el archivo de salida.\n");
                    argumentos_destruir(argumentos);
                    if(finput != stdin) fclose(finput);
                    imagen_destruir(imagen);
                    return 1;
                }
                imagen_escribir_PPM(imagen, foutput);
            }
            else
            {
                if(es_bmp(argumentos_obtener_nombre(argumentos, i)) == true)
                {
                    foutput = fopen(argumentos_obtener_nombre(argumentos, i), "wb");
                    if(foutput == NULL)
                    {
                        fprintf(stderr, "No se pudo abrir el archivo de salida.\n");
                        argumentos_destruir(argumentos);
                        if(finput != stdin) fclose(finput);
                        imagen_destruir(imagen);
                        return 1;
                    }
                    imagen_escribir_BMP(imagen, foutput);
                }
                else
                {
                    fprintf(stderr, "El archivo de salida solo puede ser .ppm o .bmp\n");
                    argumentos_destruir(argumentos);
                    if(finput != stdin) fclose(finput);
                    imagen_destruir(imagen);
                    return 1;
                }
            }
        }
    }
    if(foutput == NULL)
    {
        foutput = stdout; // En este caso no hay forma de saber si el archivo de salida va a ser .ppm o .bmp
        imagen_escribir_PPM(imagen, foutput); // Por defecto, se escribe en formato PPM
    }

    // Se libera la memoria
    argumentos_destruir(argumentos);
    imagen_destruir(imagen);

    // Cierre de archivos
    if(finput != stdin) // Si la entrada es stdin, no se cierra.
        fclose(finput);

    if(foutput != stdout) // Si la salida es stdout, no se cierra.
        fclose(foutput);
    
    return 0;
}
