#include "lista.h"
#include <stdlib.h>

typedef struct nodo nodo_t;
struct nodo{
    void* dato;
    nodo_t* siguiente;
};

struct lista{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct lista_iter{
    nodo_t* actual;
    nodo_t* anterior;
    lista_t* lista;
};

// Crea un nodo con el dato pasado por parametro, y apuntando a NULL como nodo siguiente.
nodo_t* crear_nodo(void* dato){
    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if(!nuevo_nodo) return NULL;
    nuevo_nodo->dato = dato;
    nuevo_nodo->siguiente = NULL;
    return nuevo_nodo;
}

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista) return NULL;
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    return (lista->largo == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nuevo_nodo = crear_nodo(dato);
    if (!nuevo_nodo) return false;
    if (lista_esta_vacia(lista)) lista->ultimo = nuevo_nodo;
    else nuevo_nodo->siguiente = lista->primero;
    lista->primero = nuevo_nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo(dato);
    if(!nodo) return false;
    if(lista_esta_vacia(lista)){
        lista->primero = nodo;
    }else{
        lista->ultimo->siguiente = nodo;
    }
    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void* lista_borrar_primero(lista_t *lista){
    if(lista_esta_vacia(lista)) return NULL;
    nodo_t* primero = lista->primero;
    void* dato = primero->dato;
    lista->primero = primero->siguiente;
    lista->largo--;
    free(primero);
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    return lista_esta_vacia(lista)? NULL : lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
    return lista_esta_vacia(lista)? NULL : lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    while(!lista_esta_vacia(lista)){
        void* dato =  lista_ver_primero(lista);
        if(destruir_dato){
            destruir_dato(dato);
        }
        lista_borrar_primero(lista);
    }
    free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    nodo_t* nodo_actual = lista->primero;
    if (!nodo_actual) return;
    bool aux = visitar(nodo_actual->dato, extra);
    nodo_actual = nodo_actual->siguiente;
    while(aux && nodo_actual){
        aux = visitar(nodo_actual->dato, extra);
        nodo_actual = nodo_actual->siguiente;
    }
}

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iterador = malloc(sizeof(lista_iter_t));
    if (!iterador) return NULL;
    iterador->lista = lista;
    iterador->actual = lista->primero;
    iterador->anterior = NULL;
    return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->siguiente;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    return lista_iter_al_final(iter)? NULL : iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return (!iter->actual);
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nodo_nuevo = crear_nodo(dato);
    if (!nodo_nuevo) return false;
    nodo_nuevo->siguiente = iter->actual;

    if (!(iter->anterior)){
        iter->lista->primero = nodo_nuevo;
    } 
    else {
        iter->anterior->siguiente = nodo_nuevo;
    }

    if (lista_iter_al_final(iter)) {
        iter->lista->ultimo = nodo_nuevo;
    }
    iter->actual = nodo_nuevo;
    iter->lista->largo++;
    return true; 
}

void *lista_iter_borrar(lista_iter_t *iter){
    if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) return NULL;
    nodo_t* nodo_actual = iter->actual;
    void* dato = nodo_actual->dato;

    if (!(iter->anterior)){
        iter->lista->primero = nodo_actual->siguiente;
    }else{
        iter->anterior->siguiente = iter->actual->siguiente;
    }
    if(!iter->actual->siguiente) {
        iter->lista->ultimo = iter->anterior;
    }
    iter->actual = iter->actual->siguiente;
    iter->lista->largo--;
    free(nodo_actual);
    return dato;
}