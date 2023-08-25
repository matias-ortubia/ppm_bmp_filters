#ifndef ARGUMENTOS_H
#define ARGUMENTOS_H

#define BANDERA_STR_INPUT "-input"
#define BANDERA_STR_OUTPUT "-output"
#define BANDERA_STR_FILTER "-filter"
#define BANDERA_STR_CROP "-crop"
#define BANDERA_STR_HORIZONTAL "-horizontal"
#define BANDERA_STR_VERTICAL "-vertical"

typedef struct argumentos argumentos_t;

argumentos_t *argumentos_leer(int argc, char *argv[]);
void argumentos_destruir(argumentos_t *argumentos);


/* Para cada una de las siguientes funciones, se debe ingresar un puntero a argumentos_t que haya sido leido con argumentos_leer 
 * Para las funciones con parametro "n", se debe ingresar el numero del argumento del que se quiere extraer la informacion.
 * Se considera argumento al conjunto de cadenas VALIDAS ingresadas por linea de comando, desde la bandera correspondiente hasta la siguiente
 *
 * Por ejemplo, si se ingresara:
 *       -input entrada.ppm -filtro brillo 60 -output salida.bmp
 * [-input entrada.ppm] es el argumento 0, [-filtro brillo 60] es el argumento 1, y [-output salida.bmp] es el argumento 2, y en total hay 3 argumentos. */

/* Devuelve la cantidad de argumentos cargados en memoria */
size_t argumentos_obtener_cantidad(argumentos_t *argumentos);

/* Devuelve la bandera del argumento seleccionado con "n" */
char *argumentos_obtener_bandera(argumentos_t *argumentos, size_t n);

/* Devuelve la cantidad de parametros despues de la bandera */
int argumentos_obtener_cantidad_parametros(argumentos_t *argumentos, size_t n);

/* Devuelve el nombre del filtro o del archivo que se cargo en el argumento "n" */
char *argumentos_obtener_nombre(argumentos_t *argumentos, size_t n);

/* Devuelve un entero que representa el filtro que se eligio en el argumento "n" */
int argumentos_obtener_filtro(argumentos_t *argumentos, size_t n);
/* Devuelve el parametro que podria ingresarse dependiendo del filtro elegido en el argumento "n". */
int argumentos_obtener_parametro_filtro(argumentos_t *argumentos, size_t n);

/* Devuelven el ancho/alto/parametro x/parametro y respectivamente, necesarios para la funcion de recorte, del argumento "n" */
int argumentos_obtener_ancho_recorte(argumentos_t *argumentos, size_t n);
int argumentos_obtener_alto_recorte(argumentos_t *argumentos, size_t n);
int argumentos_obtener_X_recorte(argumentos_t *argumentos, size_t n);
int argumentos_obtener_Y_recorte(argumentos_t *argumentos, size_t n);


#endif
