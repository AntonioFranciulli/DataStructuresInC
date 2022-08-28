#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_PILA_VACIA 5 // Establezco una capacidad inicial que tendra una pila al ser creada.
#define CONSTANTE_REDIMENSION 2 // Establezco una constante que se utilizara frecuentemente al realizar redimensiones de la pila.


/* Definición del struct pila proporcionado por la cátedra.
 */

typedef struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
} pila_t;


//  Función auxiliar redimensionar que modifica el tamaño de una pila, ya sea por que se apiló o se desapiló un elemento. 
//  Es utilizada tanto en la función apilar cómo desapilar.

bool redimensionar(pila_t* pila, size_t nuevo_tam){
    void** aux = realloc(pila->datos, nuevo_tam * sizeof(void*));

    if(aux == NULL) return false;

    pila->datos = aux;
    pila->capacidad = nuevo_tam;
    return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/


// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t *pila_crear(void){
    pila_t* pila = malloc(sizeof(pila_t));

    if (pila == NULL) return NULL;
    

    pila->datos = malloc(CAPACIDAD_PILA_VACIA * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }

    pila->capacidad = CAPACIDAD_PILA_VACIA;
    pila->cantidad = 0;
    return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}


// Devuelve verdadero si la pila no tiene elementos apilados, false en caso contrario.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}


// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad){
        if (!redimensionar(pila, (pila->capacidad * CONSTANTE_REDIMENSION))){
            return false;
        }
    }
    pila->datos[pila->cantidad++] = valor;
    return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void *pila_ver_tope(const pila_t *pila){
    return pila_esta_vacia(pila) ? NULL : pila->datos[pila->cantidad - 1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;
    
    if(pila->cantidad <= pila->capacidad / (CONSTANTE_REDIMENSION * CONSTANTE_REDIMENSION) && pila->cantidad > CAPACIDAD_PILA_VACIA) {
        redimensionar(pila, pila->capacidad / CONSTANTE_REDIMENSION);
    }

    pila->cantidad -= 1;
    void* dato_desapilados = pila->datos[pila->cantidad];
    pila->datos[pila->cantidad] = NULL;
    
    return dato_desapilados;
}