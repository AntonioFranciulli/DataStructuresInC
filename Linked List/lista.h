#ifndef _LISTA_H
#define _LISTA_H
#include <stdbool.h>
#include <stddef.h>

/* *************************************************************************************
 *                            DEFINICION DE LOS TIPOS DE DATOS
 * *************************************************************************************/

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* *************************************************************************************
 *                                 PRIMITIVAS DE LA LISTA
 * *************************************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: La lista fue creada.
// Post: Se agrego un nuevo dato a la lista. Dato se encuentra al principio de la misma.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un elemento al final de la lista. Devuelve falso en caso de error.
// Pre: La lista fue creada.
// Post: Se agrego un nuevo dato a la lista. Dato se encuentra al final de la misma.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista. Si la lista tiene elementos , se quita el primero
// de la lista y se devuelve su valor. Si esta vacia devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista contiene un elemento 
// menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el primer elemento de la lista si la lista tiene elementos, sino devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento, si la lista no estaba vacia.
void *lista_ver_primero(const lista_t *lista);

// Devuelve el ultimo elemento de la lista si la lista tiene elementos, sino devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el ultimo elemento, si la lista no estaba vacia.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro, llama a la misma
// para cada elemento de la lista.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* *************************************************************************************
 *                            PRIMITIVA DEL ITERADOR INTERNO
 * *************************************************************************************/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* *************************************************************************************
 *                            PRIMITIVA DEL ITERADOR EXTERNO
 * *************************************************************************************/

// Crea un iterador externo sobre la lista.
// Devuelve un iterador posicionado sobre el primer elemento de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza la posicion del iterador. Devuelve true si hay un proximo 
// elemento, false en caso contrario.
// Pre: el iterador esta creado.
// Post: el iterador apunta a la posicion siguiente, si es que existe.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el elemento al que apunta el iterador.
// Pre: el iterador esta creado. 
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador esta al final de la lista, false en caso contrario.
// Pre: el iterador esta creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador esta creado.
// Post: se destruyo el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la lista, en la posicion en la que se encuentra el iterador.
// Pre: el iterador esta creado.
// Post: Se inserto un elemento en la lista, en la posicion en la que apuntaba el iterador.
// El elemento que estaba siendo apuntado anteriormente, queda en la posicion siguiente
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Se elimina un elemento en la lista, en la posicion en la que se encuentra el iterador.
// Pre: el iterador esta creado.
// Post: Se elimina de la lista, el elemento que esta siendo apuntado por el iterador.
// El iterador queda apuntando al siguiente.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                    PRUEBAS PARA LA LISTA
 * *****************************************************************/
// Realiza pruebas sobre la implementación realizada.
void pruebas_lista_estudiante(void);

#endif  // _LISTA_H