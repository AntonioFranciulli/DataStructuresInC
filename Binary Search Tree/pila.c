#include "pila.h"

#include <stdlib.h>

#define CAPACIDAD_INICIAL 20
#define CTE_SOBREDIMENSION 4

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};


/******************************************************************
 *                      AUXILIARES
 **************************************************************** */
bool tiene_capacidad(pila_t *pila){
    return pila->cantidad < pila->capacidad;
}

bool esta_sobredimensionado(pila_t *pila){
    return pila->capacidad/CTE_SOBREDIMENSION > pila->cantidad && pila->capacidad>=CAPACIDAD_INICIAL;
}

bool redimensionar_pila(pila_t *pila, size_t nueva_capacidad){
    void* datos_nuevos = realloc(pila->datos, nueva_capacidad * sizeof(void*));
    if(datos_nuevos == NULL){
        return false;
    }
    pila->capacidad = nueva_capacidad;
    pila->datos = datos_nuevos;
    return true;
}
/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
pila_t *pila_crear(void){
    pila_t* nuevaPila = malloc(sizeof(pila_t));
    if(nuevaPila == NULL){
        return NULL;
    }
    nuevaPila->capacidad = CAPACIDAD_INICIAL;
    nuevaPila->cantidad = 0;
    nuevaPila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));

    if(nuevaPila->datos == NULL){
        free(nuevaPila);
        return NULL;
    }
    return nuevaPila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){
    if(!tiene_capacidad(pila)){
        if(!redimensionar_pila(pila,pila->capacidad*2)){
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    return pila_esta_vacia(pila) ? NULL : pila->datos[pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila){
    if(esta_sobredimensionado(pila)){
        redimensionar_pila(pila,pila->capacidad/4);
    }
    void* dato = pila_ver_tope(pila);
    if(!pila_esta_vacia(pila)){
        pila->cantidad--;
    }
    return dato;
}
