#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>



/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * **************************************************************** */

int cmp(const void *elem1, const void *elem2){
    return *(int *)elem1 - *(int *)elem2;
}

int cmp_asc(const void *elem1, const void *elem2){
    return *(int *)elem2 - *(int *)elem1;
}



/* ******************************************************************
 *                        PRUEBAS DE HEAP
 * **************************************************************** */

void prueba_crear_heap_vacio(){
    printf("Pruebas con Heap Vacío:\n");
    heap_t *heap = heap_crear(cmp);
    void** arr = NULL;
    heap_t* heap_arr = heap_crear_arr(arr,0,cmp); 

    print_test("Se pudo crear el heap vacío", heap != NULL); 
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);
    print_test("Desencolar devuelve NULL", !heap_desencolar(heap));
    print_test("ver maximo devuelve NULL", !heap_ver_max(heap));

    print_test("Se puede crear el heap a partir de un array vacio", heap_arr != NULL);
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);
    print_test("Desencolar devuelve NULL", !heap_desencolar(heap));
    print_test("Ver máximo devuelve NULL", !heap_ver_max(heap));
    heap_destruir(heap, NULL);
    heap_destruir(heap_arr,NULL);
    printf("\n");
}

void prueba_encolar_y_desencolar_pocos(){
    printf("Pruebas Encolando y Desencolando pocos elementos:\n");
    heap_t* heap = heap_crear(cmp);
    print_test("Se pudo crear el heap vacío", heap); 

    int arreglo[] = {1, 0, 7, 9, 4, 5};
    size_t tam = 6;
	
    bool ok_encolar = true;
    for (int i = 0; i < tam; i++) {
        ok_encolar &= heap_encolar(heap, &arreglo[i]);
    }
    print_test("Se encolaron varios elementos correctamente", ok_encolar);
    print_test("El heap no esta vacio", heap_esta_vacio(heap) == false);
    print_test("La cantidad de elementos en el heap es la correcta", heap_cantidad(heap) == tam);
    print_test("El máximo del heap es el elemento correcto", heap_ver_max(heap) == &arreglo[3]);
    print_test("Si desencolo el máximo, recibo el elemento correcto", heap_desencolar(heap) == &arreglo[3]);
    print_test("La cantidad de elementos tras desencolar el máximo en el heap es la correcta", heap_cantidad(heap) == tam-1);
    print_test("Luego de desencolar el máximo, el nuevo máximo es el elemento correcto", heap_ver_max(heap) == &arreglo[2]);
    
    for (int i = 0; i < tam-1; i++) {
        heap_desencolar(heap);
    }

    print_test("Se desencolaron varios elementos correctamente", true);
    print_test("El heap quedó vacio", heap_esta_vacio(heap));
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);
    heap_destruir(heap, NULL);
    printf("\n");
}

void prueba_heap_encolar_y_desencolar_valor_null(){
    printf("Pruebas Encolando y Desencolando valor Null:\n");
    heap_t* heap = heap_crear(cmp);
    print_test("Se pudo crear el heap vacío", heap);
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);
    print_test("Si desencolo el máximo, recibo el elemento Null", !heap_desencolar(heap));
    print_test("El máximo del heap es el elemento Null", !heap_ver_max(heap));
    print_test("Intento encolar un elemento Null y puedo hacerlo", heap_encolar(heap, NULL));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("El máximo del heap sigue siendo el elemento Null", !heap_ver_max(heap));
    print_test("Intento desencolar el maximo y devuelve NULL", heap_desencolar(heap) == NULL);
    heap_destruir(heap, NULL);
    printf("\n");
}

void prueba_encolar_y_desencolar_volumen(size_t largo){
    printf("Pruebas Encolando y Desencolando Alto Volumen de elementos:\n");
    void **elementos = malloc(sizeof(void *) * largo);
    bool ok = true;
    for (size_t i = 0; i < largo; i++){
        int *num = malloc(sizeof(int));
        *num = (int)i;
        elementos[i] = num;
    }
    heap_t *heap = heap_crear_arr(elementos, largo, cmp);
    print_test("Se puede crear el heap a partir de un arreglo voluminoso", heap != NULL);
    print_test("La cantidad de elementos es correcta", heap_cantidad(heap)==largo);
    print_test("El heap no esta vacio",!heap_esta_vacio(heap));
    while (!heap_esta_vacio(heap)){
        void* max = heap_ver_max(heap);
        void* data = heap_desencolar(heap);
        ok = cmp(max,data)==0;
        free(data);
    }
    print_test("Heap ver max devuelve los maximos elementos al desencolarlos",ok);
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    heap_destruir(heap,free);
    free(elementos);
    printf("\n");
}

void prueba_destruir_heap_con_free(){
    printf("Pruebas de Destrucción del Heap con Free:\n");
    heap_t* heap = heap_crear(cmp);
    print_test("Se pudo crear el heap vacío", heap); 

    int *dato0 = malloc(sizeof(int));
    int *dato1 = malloc(sizeof(int)); 
    int *dato2 = malloc(sizeof(int)); 
    int *dato3 = malloc(sizeof(int));
    int *dato4 = malloc(sizeof(int)); 
    int *dato5 = malloc(sizeof(int));

    *dato0 = 0;
    *dato1 = 1;
    *dato2 = 2;
    *dato3 = 3;
    *dato4 = 4;
    *dato5 = 5;
    
    print_test("Se encoló un primer dato", heap_encolar(heap, dato0));
    print_test("Se encoló un segundo dato", heap_encolar(heap, dato1));
    print_test("Se encoló un tercer dato", heap_encolar(heap, dato2));
    print_test("Se encoló un cuarto dato", heap_encolar(heap, dato3));
    print_test("Se encoló un quinto dato", heap_encolar(heap, dato4));
    print_test("Se encoló un sexto dato", heap_encolar(heap, dato5));

    heap_destruir(heap, free);
    print_test("Se destruyó el heap y se libero la memoria de sus elementos", true);
    printf("\n");
}

void pruebas_heapsort(size_t cant){
    printf("Pruebas Heapsort:\n");
    void **elementos = malloc(sizeof(void *) * cant);
    for (int i = 0; i < cant; i++){
        int *num = malloc(sizeof(int));
        *num = i;
        elementos[i] = num;
    }
    heap_sort(elementos, cant, cmp);
    bool ok = true;
    for (size_t j = 0; j < cant - 1; j++){
        ok = *(int *)elementos[j] < *(int *)elementos[j + 1];
        if(!ok)break;
    }
    print_test("Se ordenaron los elementos de menor a mayor.", ok);

    heap_sort(elementos,cant,cmp_asc);
    for (size_t j = 0; j < cant - 1; j++){
        ok = *(int *)elementos[j] > *(int *)elementos[j + 1];
        if(!ok)break;
    }
    print_test("Se ordenaron los elementos de mayor a menor.", ok);

    for (size_t k = 0; k < cant; k++){
        free(elementos[k]);
    }
    free(elementos);
    printf("\n");
}

void pruebas_heap_estudiante() {
    prueba_crear_heap_vacio();
    prueba_encolar_y_desencolar_pocos();
    prueba_heap_encolar_y_desencolar_valor_null();
    prueba_encolar_y_desencolar_volumen(400000);
    pruebas_heapsort(12000);
    prueba_destruir_heap_con_free();
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif