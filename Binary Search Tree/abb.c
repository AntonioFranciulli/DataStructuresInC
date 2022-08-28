#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "abb.h"
#include "pila.h"

/* ******************************************************************
 *                  DEFINICIÓN DE TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo nodo_t;
struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t comparador;
    abb_destruir_dato_t destructor;
};

struct nodo{
    nodo_t* izquierdo;
    nodo_t* derecho;
    char* clave;
    void* dato;
};


/* ******************************************************************
 *                        FUNCIÓNES AUXILIARES
 * *****************************************************************/

// Crea un nodo con la clave y el dato pasados por parámetro.
// Post: Devuelve el nodo creado.
nodo_t* nodo_crear(const char* clave, void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->clave = strdup(clave);
    nodo->dato = dato;
    nodo->izquierdo = NULL;
    nodo->derecho = NULL;
    return nodo;
}

// Libera los recursos utilizados por el nodo.
// Post: Devuelve el dato del nodo eliminado.
void* nodo_destruir(nodo_t* nodo){
    if(!nodo) return NULL;
    void* dato = nodo->dato;
    free(nodo->clave);
    free(nodo);
    return dato;
}

// Busca una clave en el árbol binario de busqueda. 
// Post: Devuelve la dirección de memoria donde debería encontrarse la clave.
nodo_t** buscar_clave(const abb_t* arbol, nodo_t** raiz, const char* clave){
    if(!*raiz) return raiz;
    if(arbol->comparador((*raiz)->clave,clave) == 0){
        return raiz;
    }
    if(arbol->comparador((*raiz)->clave,clave) > 0){
        return buscar_clave(arbol, &(*raiz)->izquierdo,clave);
    }
    return buscar_clave(arbol, &(*raiz)->derecho,clave);
}

// Devuelve la cantidad de hijos de un nodo.
// Pre: El nodo debe existir.
size_t cantidad_hijos(nodo_t* nodo){
    size_t cantidad = 0;
    if(nodo->izquierdo) cantidad++;
    if(nodo->derecho) cantidad++;
    return cantidad;
}

// Busca un reemplazo para realizar el borrado con dos hijos.
// Se debe liberar la memoria devuelta.
// Pre: El nodo debe existir
nodo_t** buscar_reemplazo(nodo_t** raiz){
    nodo_t** reemplazo = malloc(sizeof(nodo_t*));
    *reemplazo = (*raiz)->derecho;
    while ((*reemplazo)->izquierdo){
        (*reemplazo) = (*reemplazo)->izquierdo;
    }
    return reemplazo;
}

// Borra del árbol binario de busqueda un nodo con uno o ningún hijo y devuelve el dato asociado.
// Pre: el nodo debe existir y tener un hijo o ninguno.
void* borrar_con_un_hijo(abb_t* arbol, nodo_t** posicion){
    void* dato = (*posicion)->dato;
    nodo_t** hijo = malloc(sizeof(nodo_t*));
    !(*posicion)->izquierdo ? memcpy(hijo,&(*posicion)->derecho, sizeof(nodo_t*)):
                              memcpy(hijo,&(*posicion)->izquierdo, sizeof(nodo_t*));
    bool es_raiz = arbol->raiz == *posicion;
    nodo_destruir(*posicion);
    *posicion = *hijo;
    if(es_raiz) arbol->raiz = *hijo;
    free(hijo);
    arbol->cantidad--;
    return dato;
}

// Borra del árbol binario de busqueda un nodo con dos hijos y devuelve el dato asociado.
// Pre: el nodo debe existir y tener exactamente dos hijos.
void* borrar_con_dos_hijos(abb_t* arbol, nodo_t** posicion){
    nodo_t** reemplazo = buscar_reemplazo(posicion);
    free((*posicion)->clave);
    void* dato = (*posicion)->dato;
    (*posicion)->clave = strdup((*reemplazo)->clave);
    (*posicion)->dato = (*reemplazo)->dato;
    free(reemplazo);
    reemplazo = buscar_clave(arbol,&(*posicion)->derecho,(*posicion)->clave);
    borrar_con_un_hijo(arbol,reemplazo);
    return dato;
}

// Recorre el árbol binario de busqueda in order y le aplica la función visitar a cada elemento.
bool in_order(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra){
    if(!nodo) return true;
    if(!in_order(nodo->izquierdo,visitar,extra)) return false;
    if(!visitar(nodo->clave,nodo->dato,extra))return false;
    return in_order(nodo->derecho,visitar,extra);
}

// Destruye los elementos del árbol binario de busqueda
void destruir(abb_t* arbol, nodo_t *raiz){
    if(!raiz) return;
    destruir(arbol,raiz->izquierdo);
    destruir(arbol,raiz->derecho);
    void* dato = nodo_destruir(raiz);
    if(arbol->destructor) arbol->destructor(dato);
}


/* ******************************************************************
 *                      PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->raiz = NULL;
    abb->comparador = cmp;
    abb->destructor = destruir_dato;
    abb->cantidad = 0;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t** posicion = buscar_clave(arbol,&arbol->raiz,clave);
    if(*posicion){
        if(arbol->destructor) arbol->destructor((*posicion)->dato);
        (*posicion)->dato = dato;
        return true;
    }
    *posicion = nodo_crear(clave,dato);
    if(!*posicion) return false;
    arbol->cantidad++;
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    nodo_t** posicion = buscar_clave(arbol,&arbol->raiz,clave);
    if(!*posicion) return NULL;
    if(cantidad_hijos(*posicion) < 2){
        return borrar_con_un_hijo(arbol,posicion);
    }
    return borrar_con_dos_hijos(arbol,posicion);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_t* raiz = arbol->raiz;
    nodo_t** referencia = buscar_clave(arbol,&raiz,clave);
    if(!*referencia) return NULL; 
    return (*referencia)->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_t* raiz = arbol->raiz;
    return *buscar_clave(arbol,&raiz,clave) != NULL;
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    destruir(arbol,arbol->raiz);
    free(arbol);
}


/* ******************************************************************
 *                          ITERADORES
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    in_order(arbol->raiz,visitar,extra);
}

struct abb_iter{
    pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iterador = malloc(sizeof(abb_iter_t*));
    if(!iterador) return NULL;
    iterador->pila = pila_crear();
    if(!iterador->pila){
        free(iterador);
        return NULL;
    }
    if(arbol->raiz){
        pila_apilar(iterador->pila,arbol->raiz);
        nodo_t* hijo_izq = arbol->raiz->izquierdo;
        while (hijo_izq){
            pila_apilar(iterador->pila,hijo_izq);
            hijo_izq = hijo_izq->izquierdo;
        }
    }
    return iterador;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(!abb_iter_in_al_final(iter)){
        nodo_t* actual = (nodo_t*)pila_desapilar(iter->pila);
        if(actual->derecho){
            pila_apilar(iter->pila,actual->derecho);
            nodo_t* hijo_izq = actual->derecho->izquierdo;
            while (hijo_izq){
                pila_apilar(iter->pila,hijo_izq);
                hijo_izq = hijo_izq->izquierdo;
            }
        } 
        
        return true;
    }
    return false;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    nodo_t* actual = (nodo_t*)pila_ver_tope(iter->pila);
    if(!actual) return NULL;
    return actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}