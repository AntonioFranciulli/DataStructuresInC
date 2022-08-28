#include <stdlib.h>
#include <stdbool.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* Se trata de un arbol binario de busqueda que contiene datos de tipo 
 * void* (punteros genéricos).  La estructura interna esta en el .c.  */
typedef struct abb abb_t;

/* Funcion de comparacion utilizada para el orden el el ABB
 * (punteros genéricos).  La pila en sí está definida en el .c.  */
typedef int (*abb_comparar_clave_t) (const char *, const char *);

// Funcion utilizada para borrar los datos del arbol.
typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

// Crea un arbol.
// Post: devuelve un ABB vacio.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Agrega un nuevo elemento al ABB. Devuelve falso en caso de error.
// Pre: El ABB fue creado.
// Post: se agregó un nuevo elemento al ABB.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Si la clave esta en el ABB, la saca del mismo y devuelve su valor asociado.
// Si la clave no esta en el ABB devuelve NULL.
// Pre: el ABB fue creado.
// Post: si la clave estaba en el ABB, se devuelve el valor asociado
// y el arbol contiene un elemento menos.
void *abb_borrar(abb_t *arbol, const char *clave);

// Si la clave esta en el ABB, devuelve el valor asociado sin 
// modificar el ABB. 
// Si la clave no esta en el ABB, devuelve NULL.
// Pre: El ABB fue creado.
// Post: se devolvio el valor asociado a la clave, cuando la clave está
// en el ABB, NULL en caso contrario.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Si la clave esta en el ABB, devuelve TRUE. Devuelve FALSE en caso contrario.
// Pre: El ABB fue creado.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de elementos almacenados en el ABB.
// Pre: el arbol fue creado.
size_t abb_cantidad(const abb_t *arbol);

// Destruye el ABB.
// Pre: El ABB fue creado.
// Post: se eliminaron todos los elementos del ABB.
void abb_destruir(abb_t *arbol);


/* ******************************************************************
 *                         ITERADOR INTERNO
 * *****************************************************************/
// Aplica la funcion visitar a cada elemento del ABB, in order.
// Pre: El ABB fue creado.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);
/* ******************************************************************
 *                         ITERADOR EXTERNO
 * *****************************************************************/

// Definicion del iterado externo.
typedef struct abb_iter abb_iter_t;

// Crea un iterador externo sobre el ABB.
// Devuelve un iterador posicionado sobre la raiz del ABB.
// Pre: El ABB fue creado.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza la posicion del iterador. Devuelve true si hay un proximo 
// elemento, false en caso contrario.
// Pre: el iterador esta creado.
// Post: el iterador apunta a la posicion siguiente, si es que existe.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve la clave del elemento que apunta el iterador.
// Pre: el iterador esta creado. 
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve true si el iterador recorrio todo el ABB, false en caso contrario.
// Pre: el iterador esta creado.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador esta creado.
// Post: se destruyo el iterador.
void abb_iter_in_destruir(abb_iter_t* iter);
