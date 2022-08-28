#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * **************************************************************** */



//  Pruebas de condiciones de borde que se reutilizan en distintas pruebas.
static void prueba_condiciones_borde(pila_t* pila){
    print_test("-> El tope de la pila vacía es NULL.", pila_ver_tope(pila) == NULL);
    print_test("-> Se intentó desapilar la pila vacía y no se pudo.", pila_desapilar(pila) == NULL);
    print_test("-> No se puede desapilar elementos de esta pila.", pila_desapilar(pila) == NULL);
    print_test("-> Ver_tope en esta pila vacía o recién creada es NULL.", pila_ver_tope(pila) == NULL);
    print_test("-> La pila está vacía.", pila_esta_vacia(pila));
}

//  Pruebas generales (Creación y destrucción de una pila, casos borde en pila vacía y apilar valor Null a una pila).
static void prueba_pila_vacia(void){
    printf("[A] - Pruebas de creación y destrucción de pilas: \n");
    pila_t* pila = pila_crear();
    void* aux = NULL;
    print_test("-> Se crea una pila vacía.", pila != NULL);
    prueba_condiciones_borde(pila);
    pila_apilar(pila, aux);
    print_test("-> Se apilo correctamente un elemento NULL a la pila.", pila_desapilar(pila) == NULL);
    pila_destruir(pila);
    print_test("-> Se destruyó correctamente  la pila vacía.", true);
    printf("\n");

}

//	Pruebas de pila apilando y desapilando algunos elementos manteniendo la invariante y pruebas de casos borde en una pila recién creada y recién vaciada.
static void prueba_apilar_y_desapilar(){
	printf("[B] - Pruebas de apilar y desapilar elementos manteniendo la invariante: \n");
	pila_t* pila_elementos = pila_crear();
	print_test("-> Se crea una pila vacía.", pila_elementos != NULL);
	int arreglo[] = {0, 1, 2, 3}; int i = 0;

    //  Pruebo condiciones de borde en una pila recién creada.
	prueba_condiciones_borde(pila_elementos);

	// Pruebo apilar sobre una pila recien creada.

	print_test("-> Se apilo el elemento 1 con exito.", pila_apilar(pila_elementos, &arreglo[i]));
	print_test("-> Se verifico que elemento 1 es el tope de la pila.", pila_ver_tope(pila_elementos) == &arreglo[i]);
	print_test("-> Se apilo el elemento 2 con exito.", pila_apilar(pila_elementos, &arreglo[++i]));
	print_test("-> Se verifico que elemento 2 es el tope de la pila.", pila_ver_tope(pila_elementos) == &arreglo[i]);
	print_test("-> La pila no se encuentra vacía.", !pila_esta_vacia(pila_elementos));
	print_test("-> Se apilo el elemento 3 con exito.", pila_apilar(pila_elementos, &arreglo[++i]));
	print_test("-> Se verifico que elemento 1 es el tope de la pila.", pila_ver_tope(pila_elementos) == &arreglo[i]);
	print_test("-> Se apilo el elemento 4 con exito.", pila_apilar(pila_elementos, &arreglo[++i]));

	//  Pruebo comportamiento sobre una pila con algunos elementos en ella.
	print_test("-> La pila no se encuentra vacía.", !pila_esta_vacia(pila_elementos));
	print_test("-> El tope es la referencia al elemento 4.", pila_ver_tope(pila_elementos) == &arreglo[i]);
	print_test("-> Se esapilo referencia al elemento 4.", pila_desapilar(pila_elementos) == &arreglo[i]);
	print_test("-> El tope es la referencia al elemento 3.", pila_ver_tope(pila_elementos) == &arreglo[--i]);
	print_test("-> Se desapilo referencia al elemento 3.", pila_desapilar(pila_elementos) == &arreglo[i]);
	print_test("-> La pila no se encuentra vacía.", !pila_esta_vacia(pila_elementos));
	print_test("-> El tope es la referencia al elemento 2.", pila_ver_tope(pila_elementos) == &arreglo[--i]);
	print_test("-> Se desapilo referencia al elemento 2.", pila_desapilar(pila_elementos) == &arreglo[i]);
	print_test("-> El tope es la referencia al elemento 1.", pila_ver_tope(pila_elementos) == &arreglo[--i]);
	print_test("-> Se desapilo referencia al elemento 1.", pila_desapilar(pila_elementos) == &arreglo[i]);
    
    //  Pruebo condiciones borde en una pila recíen vaciada.
    prueba_condiciones_borde(pila_elementos);

	pila_destruir(pila_elementos);
    printf("\n");
}

//	Pruebas de volumen en una pila.
static void prueba_apilar_y_desapilar_volumen(){
    printf("[C] - Pruebas de apilar y desapilar un alto volumen de elementos: \n");
	pila_t* pila_volumen = pila_crear();
    print_test("-> Se crea una pila vacía.", pila_volumen != NULL);

	int tam = 10000;
	int *arreglo = malloc(tam * sizeof(int));
	if(!arreglo) {
		print_test("CUIDADO -> Falló el malloc en la prueba de volumen de memoria y se cancelaron el resto de pruebas.", false);
		return;
	}

	bool ok_apilar = true; bool ok_tope = true;
	for(int i = 0; i < tam; i++){
		ok_apilar &= pila_apilar(pila_volumen, &arreglo[i]);
		ok_tope &= (pila_ver_tope(pila_volumen) == &arreglo[i]);
		if ((!ok_apilar) || (!ok_tope)) break;
	}

	print_test("-> Se apilaron 10.000 elementos correctamente.", ok_apilar);
    print_test("-> Se verificaron que los cada elemento apilado fue tope de la pila.", ok_tope);


    //  Desapilo todos los elementos que he apilado y luego realizo ciertas pruebas para verificar que la pila haya quedado vacía.
	bool ok_desapilar = true;
	int i = tam-1;
	while (!pila_esta_vacia(pila_volumen)) {
		ok_desapilar &= (pila_desapilar(pila_volumen) == &arreglo[i]);
		if (!ok_desapilar) break;
		i --;
		if (!pila_esta_vacia(pila_volumen)) {
			ok_tope &= (pila_ver_tope(pila_volumen) == &arreglo[i]);
			if (!ok_tope) break;
		}
	}

    print_test("-> Se desapilaron 10.000 elementos correctamente.", ok_desapilar);
    pila_destruir(pila_volumen);
	free(arreglo);
    printf("\n");
}

void pruebas_pila_estudiante(){
    prueba_pila_vacia();
    prueba_apilar_y_desapilar();
    prueba_apilar_y_desapilar_volumen();
}

/*
 * Función main() que llama a la función de pruebas.

*/

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void){
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
