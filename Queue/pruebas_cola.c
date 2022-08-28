#include "cola.h"
#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


/* Funcion auxiliar para probar cola_destruir con puntero a función. */

void pila_destruir_wrapper(void *pila){
	pila_destruir(pila);
}



/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * **************************************************************** */

//  Pruebas de condiciones de borde que se reutilizan en distintas pruebas.
static void prueba_condiciones_borde(cola_t* cola) {
    print_test("-> El primer elemento de la cola vacía es NULL.", cola_ver_primero(cola) == NULL);
    print_test("-> Se intentó desencolar la cola vacía y no se pudo.", cola_desencolar(cola) == NULL);
    print_test("-> Tras intentar desencolar, cola_ver_primero en una cola vacía es NULL.", cola_ver_primero(cola) == NULL);
    print_test("-> La cola está vacía.", cola_esta_vacia(cola));
}

//  Pruebas con una cola vacía.
static void prueba_cola_vacia() {
    printf("[A] - Pruebas sobre cola vacia:\n");
    cola_t* cola = cola_crear();
    print_test("-> Se crea una cola vacía.", cola != NULL);

    //  Pruebo condiciones de borde en una cola recién creada.
    prueba_condiciones_borde(cola);
 
    //  Destruyo la cola.
    cola_destruir(cola, NULL);
    print_test("-> Se destruyó la cola correctamente.", true);
    printf("\n");
}

// Pruebas de cola encolando y desencolando algunos elementos manteniendo la invariante y pruebas de casos borde en una cola recién creada y recién vaciada.
static void prueba_encolar_y_desencolar_pocos() {
	printf("[B] - Pruebas de encolar y desencolar elementos manteniendo la invariante:\n");
	cola_t* cola = cola_crear();
	print_test("-> Se crea una cola vacía.", cola != NULL);
	int arreglo[] = {0, 1, 2, 3}; int i = 0;

    //  Pruebo condiciones de borde en una cola recién creada.
	prueba_condiciones_borde(cola);

	// Pruebo encolar sobre una cola recien creada.
	print_test("-> Se encolo el elemento 1 con exito.", cola_encolar(cola, &arreglo[i]));
	print_test("-> Se verifico que elemento 1 es el tope de la cola.", cola_ver_primero(cola) == &arreglo[0]);
	print_test("-> Se encolo el elemento 2 con exito.", cola_encolar(cola, &arreglo[++i]));
	print_test("-> Se verifico que elemento 1 sigue siendo el primero de la cola.", cola_ver_primero(cola) == &arreglo[0]);
	print_test("-> La cola no se encuentra vacía.", !cola_esta_vacia(cola));
	print_test("-> Se encolo el elemento 3 con exito.", cola_encolar(cola, &arreglo[++i]));
	print_test("-> Se verifico que elemento 1 sigue siendo el primero de la cola.", cola_ver_primero(cola) == &arreglo[0]);
	print_test("-> Se encolo el elemento 4 con exito.", cola_encolar(cola, &arreglo[++i]));
	print_test("-> Se verifico que elemento 1 sigue siendo el primero de la cola.", cola_ver_primero(cola) == &arreglo[0]);

	//  Pruebo comportamiento sobre una cola con algunos elementos en ella.
	size_t j = 0;
	print_test("-> La cola no se encuentra vacía.", !cola_esta_vacia(cola));
	print_test("-> El primero de la cola es la referencia al elemento 1.", cola_ver_primero(cola) == &arreglo[j]);
	print_test("-> Se desencolo el elemento 1 con exito.", cola_desencolar(cola));
	print_test("-> El primero de la cola es la referencia al elemento 2.", cola_ver_primero(cola) == &arreglo[++j]);
	print_test("-> La cola no se encuentra vacía.", !cola_esta_vacia(cola));
	print_test("-> Se desencolo el elemento 2 con exito.", cola_desencolar(cola));
	print_test("-> El primero de la cola es la referencia al elemento 3.", cola_ver_primero(cola) == &arreglo[++j]);
	print_test("-> Se desencolo el elemento 3 con exito.", cola_desencolar(cola));
	print_test("-> El primero de la cola es la referencia al elemento 4.", cola_ver_primero(cola) == &arreglo[++j]);
	print_test("-> Se desencolo el elemento 4 con exito.", cola_desencolar(cola));
    
    //  Pruebo condiciones borde en una cola recíen vaciada.
    prueba_condiciones_borde(cola);

	//  Destruyo la cola.
	cola_destruir(cola, NULL);
	print_test("-> Se destruyó la cola correctamente.", true);
    printf("\n");
}

// Pruebas de volumen en una cola.
static void prueba_encolar_y_desencolar_volumen() {
    printf("[C] - Pruebas de encolar y desencolar un alto volumen de elementos:\n");
    cola_t* cola = cola_crear();
    print_test("-> Se crea una cola vacía.", cola != NULL);

    unsigned int tam = 10000;
    int *arreglo = malloc(tam * sizeof(int));
    if(!arreglo) {
        print_test("CUIDADO -> Falló el malloc en la prueba de volumen de memoria y se cancelaron el resto de pruebas.", false);
        return;
    }

	bool ok_encolar = true;
	for(int i = 0; i < tam; i++){
        ok_encolar &= cola_encolar(cola, &arreglo[i]);
        if (!ok_encolar) break;
    }

    print_test("-> Se encolaron 10.000 elementos correctamente.", ok_encolar);

    //  Desencolo todos los elementos que he encolado y tras desencolar cada uno verifico que el siguiente en la fila es el correspondiente
	bool ok_desencolar = true; bool ok_ver_primero = true;
    int j = 0;
    while (!(j == tam)) {
        ok_desencolar &= (cola_desencolar(cola) == &arreglo[j]);
        if (!ok_desencolar) break;
        j++;
        if (!(j == tam)) {
        	ok_ver_primero &= (cola_ver_primero(cola) == &arreglo[j]);
        	if (!ok_ver_primero) break;
        }
    }

    print_test("-> Se desencolaron 10.000 elementos correctamente.", ok_desencolar);
    print_test("-> Se verificaron que cada elemento encolado fue primero en la cola en algún momento.", ok_ver_primero);

    //  Pruebo condiciones borde en una cola recíen vaciada y destruyo la cola.
    prueba_condiciones_borde(cola);
    cola_destruir(cola, NULL);
    free(arreglo);
    print_test("-> Se destruyó la cola correctamente.", true);
    printf("\n");
}

// Pruebas de una cola que es destruida sin modificar los elementos en un arreglo.
static void prueba_destruir_sin_funcion_de_destruccion(){
    printf("[D] - Pruebas de destrucción sin una función de destrucción 2:\n");
    cola_t* cola = cola_crear();
    print_test("-> Se crea una cola vacía.", cola != NULL);

    int tam = 50; int arreglo[tam];
    for (int i = 0; i < tam; i++) {
    	arreglo[i] = i;
    }

    //  Pruebo condiciones de borde en una cola recién creada.
    prueba_condiciones_borde(cola);

    //  Encolo cada elemento del arreglo.
    bool ok_encolar = true;
    for (int i = 0; i < tam; i++) {
        ok_encolar &= cola_encolar(cola, &arreglo[i]);
    }

    print_test("-> Se encolaron los elementos del arreglo correctamente.", ok_encolar);
    print_test("-> Despues de encolar elementos del arreglo la cola no se encuentra vacía", !cola_esta_vacia(cola));

    //	Pruebo destruir la cola sin modificar los datos almacenados.
    cola_destruir(cola, NULL);
    bool datos_no_modificados = true;
    for (int i = 0; i < tam; i++) {
        datos_no_modificados &= arreglo[i] == i;
    }

    print_test("-> Los datos del arreglo no fueron modificados", datos_no_modificados);
    print_test("-> Se destruyó la cola correctamente.", true);
    printf("\n");
}

// Pruebas de una cola de punteros pedidos con malloc().
static void prueba_destruir_con_funcion_de_destruccion_1(){
	printf("[E] - Pruebas de destrucción con una función de destrucción 1:\n");
	cola_t* cola = cola_crear();
	print_test("-> Se crea una cola vacía.", cola != NULL);

	int* elemento_1 = malloc(sizeof(int));
	char* elemento_2 = malloc(sizeof(char));
	bool* elemento_3 = malloc(sizeof(float));
	float* elemento_4 = malloc(sizeof(float));
	double* elemento_5 = malloc(sizeof(float));

    //  Pruebo condiciones de borde en una cola recién creada.
    prueba_condiciones_borde(cola);

    // Encolo cada elemento.
    print_test("-> Encolar referencia al elemento 1.", cola_encolar(cola, elemento_1));
    print_test("-> El primero es la referencia al elemento 1.", cola_ver_primero(cola) == elemento_1);
    print_test("-> Encolar referencia al elemento 2.", cola_encolar(cola, elemento_2));
    print_test("-> El primero sigue siendo la referencia al elemento 1.", cola_ver_primero(cola) == elemento_1);
    print_test("-> Encolar referencia al elemento 3.", cola_encolar(cola, elemento_3));
    print_test("-> El primero sigue siendo la referencia al elemento 1.", cola_ver_primero(cola) == elemento_1);
    print_test("-> Encolar referencia al elemento 4.", cola_encolar(cola, elemento_4));
    print_test("-> El primero sigue siendo la referencia al elemento 1.", cola_ver_primero(cola) == elemento_1);
    print_test("-> Encolar referencia al elemento 5.", cola_encolar(cola, elemento_5));
    print_test("-> El primero sigue siendo la referencia al elemento 1.", cola_ver_primero(cola) == elemento_1);

    cola_destruir(cola, free);
    print_test("-> Se destruyó la cola correctamente.", true);

    cola_t* cola2 = cola_crear();
    print_test("-> Se crea una cola vacía.", cola2 != NULL);
    cola_destruir(cola2, free);
    print_test("-> Se destruyó la cola vacía correctamente.", true);
    printf("\n");
}

//  Pruebas de destrucción de una cola con pilas encoladas que tambien deben ser destruidos mediante puntero a función.
static void prueba_destruir_con_funcion_de_destruccion_2(){
	printf("[F] - Pruebas de destrucción con una función de destrucción 2:\n");
	cola_t* cola = cola_crear();
	print_test("-> Se crea una cola vacía.", cola_esta_vacia(cola));

	for (int i = 0; i < 5; i++) {
		pila_t* pila = pila_crear();
		for (int j = 0; j < 5; j++) {
			pila_apilar(pila, &j);
		}
		cola_encolar(cola, pila);
	}

	print_test("-> Se apilaron elementos en las pilas.", true);
	print_test("-> Se apilaron las pilas en la cola.", !cola_esta_vacia(cola));


	cola_destruir(cola, pila_destruir_wrapper);
    print_test("-> Se eliminaron los elementos de la cola destruyendo tambien las pilas encoladas.", true);
}

void pruebas_cola_estudiante() {
    prueba_cola_vacia();
    prueba_encolar_y_desencolar_pocos();
    prueba_encolar_y_desencolar_volumen();
    prueba_destruir_sin_funcion_de_destruccion();
    prueba_destruir_con_funcion_de_destruccion_1();
    prueba_destruir_con_funcion_de_destruccion_2();
}

/*
 * Función main() que llama a la función de pruebas.

*/

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
