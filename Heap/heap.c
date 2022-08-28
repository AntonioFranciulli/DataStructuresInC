#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#define LARGO_INICIAL 50
#define POS_INVALIDA (size_t)-1
#define EXTENDER 0
#define ENCOGER 1


/*******************************************************************************
*                               TIPOS DE DATOS
********************************************************************************/

struct heap{
    void** arreglo;
    size_t largo_arreglo;
    cmp_func_t cmp;
    size_t cant_elementos;
};



/*******************************************************************************
*                               FUNCIONES AUXILIARES
********************************************************************************/

// Intercambia dos valores opacos.
void swap(void** elemento1,void** elemento2){
    void* aux = *elemento1;
    *elemento1 = *elemento2;
    *elemento2 = aux;
}

// Devuelve la posicion del hijo izquierdo de una posicion pasada por parametro.
// En el caso de ser una posicion mayor a tam, devuelve POS_INVALIDA.
size_t obtener_pos_hijo_izquierdo(size_t n,size_t tam){
    size_t resultado = 2*n+1;
    return resultado < tam? resultado: POS_INVALIDA;
}

// Devuelve la posicion del hijo derecho de una posicion pasada por parametro.
// En el caso de ser una posicion mayor a tam, devuelve POS_INVALIDA.
size_t obtener_pos_hijo_derecho(size_t n,size_t tam){
    size_t resultado = 2*n+2;
    return resultado < tam? resultado: POS_INVALIDA;
}

// Devuelve la posicion del padre de la posicion pasada por parametro.
// En el caso del padre de 0, devuelve POS_INVALIDA.
size_t obtener_pos_padre(size_t n){
    if(n==0) return POS_INVALIDA;
    return (n-1)/2;
}

// Devuelve la posicion del mayor de los elementos pasados por parametro.
// Si ninguna posicion es valida devuelve POS_INVALIDA
size_t obtener_mayor(void** elementos, size_t pos_1,size_t pos_2,cmp_func_t cmp){
    if(pos_1 == POS_INVALIDA && pos_2 == POS_INVALIDA) return POS_INVALIDA;
    if(pos_1 == POS_INVALIDA) return pos_2;
    if(pos_2 == POS_INVALIDA) return pos_1;
    return cmp(elementos[pos_1],elementos[pos_2])<0 ? pos_2:pos_1;
}

// Verifica si los elementos en las posiciones correspondientes a los hijos del elemento del cual
// se pasa la posicion por parametro son mayores a este. En tal caso se intercambian con el mayor
// y se vuelve a aplicar downheap al mismo elemento.
void downheap(void** elementos, size_t tam ,size_t pos_padre, cmp_func_t cmp){    
    size_t hijo_der = obtener_pos_hijo_derecho(pos_padre,tam);
    size_t hijo_izq = obtener_pos_hijo_izquierdo(pos_padre,tam);
    size_t pos_mayor = obtener_mayor(elementos,hijo_izq,hijo_der,cmp);
    if(pos_mayor == POS_INVALIDA) return;
    if(cmp(elementos[pos_padre],elementos[pos_mayor])<0){
        swap(&elementos[pos_padre],&elementos[pos_mayor]);
        downheap(elementos,tam,pos_mayor,cmp);
    }
}

// Verifica si el padre de un elemento es mayor al hijo. Si no es asi, los intercambia
// y vuelve a aplicar upheap al nuevo padre.
void upheap(void** elementos,size_t pos_hijo, cmp_func_t cmp){
    size_t pos_padre = obtener_pos_padre(pos_hijo);
    while (pos_padre!= POS_INVALIDA){
        if(cmp(elementos[pos_padre],elementos[pos_hijo])>0){
            return;
        }
        swap(&elementos[pos_padre],&elementos[pos_hijo]);
        pos_hijo = pos_padre;
        pos_padre = obtener_pos_padre(pos_hijo);
    }
}

// Le da forma de heap a un arreglo, basado en la funcion de comparacion.
void heapify(void** elementos, size_t cant, cmp_func_t cmp){
    for (size_t i = cant/2; i-->0;){
       downheap(elementos,cant,i,cmp);
    }
}

// Redimensiona el heap según sea necesario.
bool redimensionar(heap_t* heap, int tipo){
    size_t nuevo_largo = tipo == EXTENDER ? heap->largo_arreglo*2: heap->largo_arreglo/2;
    void** nuevo_arreglo = realloc(heap->arreglo, sizeof(void*)*nuevo_largo);
    if(!nuevo_arreglo){
        return false;
    }
    heap->largo_arreglo = nuevo_largo;
    heap->arreglo = nuevo_arreglo;
    return true;
}



/********************************************************************************
 *                                  HEAPSORT
**********************************************************************************/

// Ordena un arreglo de menor a mayor, con el criterio de la funcion de comparacion.
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos,cant,cmp);
    for (size_t i = cant; i -->0;)
    {
        swap(&elementos[0],&elementos[i]);
        downheap(elementos,i,0,cmp);   
    }
}



/*****************************************************************************************
*                                   PRIMITIVAS DEL HEAP
******************************************************************************************/
heap_t *heap_crear(cmp_func_t cmp){
    return heap_crear_arr(NULL,0,cmp);
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    size_t largo = n > 0? n : LARGO_INICIAL;
    heap->arreglo = malloc(sizeof(void*)*largo);
    if(!heap->arreglo){
        free(heap);
        return NULL;
    }
    heap->cant_elementos = n > 0? largo : 0;
    heap->cmp = cmp;
    heap->largo_arreglo = largo;
    if(n>0){
        for (size_t i = 0; i < largo; i++){
            heap->arreglo[i] = arreglo[i];
            upheap(heap->arreglo,i,cmp);
        }
    }
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento){
        for (size_t i = 0; i < heap->cant_elementos; i++){
            destruir_elemento(heap->arreglo[i]);
        }
    }
    free(heap->arreglo);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant_elementos;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cant_elementos == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
    if(heap->cant_elementos == heap->largo_arreglo){
         if(!redimensionar(heap,EXTENDER)) return false;
    }
    heap->arreglo[heap->cant_elementos] = elem;
    upheap(heap->arreglo,heap->cant_elementos,heap->cmp);
    heap->cant_elementos++;
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    void* dato = heap->arreglo[0];
    heap->arreglo[0] = heap->arreglo[heap->cant_elementos-1];
    downheap(heap->arreglo,heap->cant_elementos,0,heap->cmp);
    heap->cant_elementos--;

    if(heap->largo_arreglo/4>heap->cant_elementos && heap->largo_arreglo >LARGO_INICIAL){
        redimensionar(heap,ENCOGER);
    }
    return dato;
}