#include "cola.h"
#include <stdlib.h>

/*  Definición del struct cola y nodo.*/

typedef struct nodo {
	void* dato;
	struct nodo* prox;
} cola_nodo_t;

struct cola {
	cola_nodo_t* primero;
	cola_nodo_t* ultimo;
};

/*  Funciones auxiliares & primitivas del struct Nodo. */

//  Destruye un nodo.
//  Pre: El nodo existe.
//  Post: Se liberó la memoria que ocupaba el nodo.
void destruir_nodo(cola_nodo_t* nodo){
    free(nodo);
}

//  Crea un nodo.
//  Post: Devuelve un nodo con su dato almacenado.
cola_nodo_t* crear_nodo(void* valor){
    cola_nodo_t* nuevo_nodo = malloc(sizeof(cola_nodo_t));
    if (nuevo_nodo == NULL) return NULL;
    nuevo_nodo->dato = valor;
    nuevo_nodo->prox = NULL;
    return nuevo_nodo;
}


/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t *cola_crear(void){
    cola_t* cola = malloc(sizeof(cola_t));
    if (cola == NULL) return NULL;
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
    while(!cola_esta_vacia(cola)){
        void* dato = cola_desencolar(cola);
        if (destruir_dato) (*destruir_dato)(dato);
    }
    free(cola);
}

// Devuelve verdadero si la cola no tiene elementos encolados, false en caso contrario.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola){
    return (cola->primero == NULL && cola->ultimo == NULL); 
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void *valor){
    cola_nodo_t* nuevo_nodo = crear_nodo(valor);
    if (nuevo_nodo == NULL) return false;
    if (cola_esta_vacia(cola)){
        cola->primero = nuevo_nodo;
    } else {
        cola->ultimo->prox = nuevo_nodo;
    }
    cola->ultimo = nuevo_nodo;
    return true;
}

// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void *cola_ver_primero(const cola_t *cola){
    if (cola_esta_vacia(cola)) return NULL;
    return cola->primero->dato;
}

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void* cola_desencolar(cola_t *cola){
    if (cola_esta_vacia(cola)) return NULL;
    void* dato_aux = cola_ver_primero(cola);
    cola_nodo_t* nodo_aux = cola->primero;
    cola->primero = cola->primero->prox;
	if (!cola->primero) cola->ultimo = NULL;
	destruir_nodo(nodo_aux);
	return dato_aux;
}