#include "lista.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>


//  Funcion auxiliar que busca la primera aparicion de un numero primo y devuelve su posicion en la lista.
bool es_primo(void* dato, void* numero_primo){
    int* numero = dato;
    int* numero_primo_encontrado = numero_primo;
    int aux = 0;
    for (int i = 1; i <= *numero; ++i) {   
        if (*numero % i == 0) {
            aux++;
        }
    }      
    if (aux == 2){
        numero_primo_encontrado = numero;
        print_test("Se ha encontrado un numero primo", true);
        printf("El numero primo encontrado fue: %d\n", *numero_primo_encontrado);
        return false;    
    }
    else {
        print_test("No se ha encontrado un numero primo", true);
        return true;
    }
}

// Funcion auxiliar que multiplica un valor por otro (utilizada en las pruebas de iterador interno sin corte).
bool multiplicar(void* dato, void* extra) {
    int* aux = dato;
    *(int *) extra *= *aux;
    return true;
}

void pruebas_iterador_interno_con_corte(){
    printf("%s","PRUEBAS DE ITERADOR INTERNO CON CORTE\n");
    int arreglo[] = {1, 1, 1, 1, 6, 3, 1, 8, 29, 17};
    lista_t* lista = lista_crear();
    size_t tam_arreglo = 10;
    for (int i = 0; i < tam_arreglo; i++){
        lista_insertar_ultimo(lista, &arreglo[i]);
    };
    printf("Prueba iterar una lista hasta encontrar la primera aparición de un numero primo\n");
    int numero_primo = 0;
    lista_iterar(lista, es_primo, &numero_primo);
    lista_destruir(lista, NULL);
    printf("\n");
}

void pruebas_iterador_interno_sin_corte(){
    printf("%s","PRUEBAS DE ITERADOR INTERNO SIN CORTE\n");
    int arreglo[] = {4, 2, 6, 4, 6, 2, 1, 2, 1, 5};
    lista_t* lista = lista_crear();
    size_t tam_arreglo = 10;
    for (int i = 0; i < tam_arreglo; i++){
        lista_insertar_ultimo(lista, &arreglo[i]);
    };
    printf("Prueba iterar una lista multiplicando entre si todos sus elementos\n");
    int resultado = 1;
    lista_iterar(lista, multiplicar, &resultado);
    print_test("Se multiplicaron correctamente todos los elementos entre si de la lista", 23040 == resultado);
    printf("Resultado buscado: 23040\n");
    printf("Resultado obtenido: %d\n", resultado);
    lista_destruir(lista, NULL);
    printf("\n");
}

void iterador_insertar_al_principio(){
    lista_t* lista = lista_crear();
    lista_iter_t* iterador = lista_iter_crear(lista);
    int elem = 8;

    print_test("El iterador externo se pudo crear", iterador);
    print_test("Intentar avanzar el iterador en una lista vacía falla", lista_iter_avanzar(iterador) == false);
    print_test("Ver el el elemento actual del iterador es NULL en una lista vacía", lista_iter_ver_actual(iterador) == NULL);

    lista_iter_insertar(iterador, &elem);
    print_test("Al insertar un elemento en la posición en la que se crea el iterador, efectivamente se inserta al principio", lista_iter_ver_actual(iterador) == lista_ver_primero(lista));
    
    lista_iter_destruir(iterador);
    lista_destruir(lista,NULL);
}

void iterador_remover_al_principio(int* arreglo,size_t longitud_arreglo){
    lista_t* lista = lista_crear();

    for (size_t i = 0; i < longitud_arreglo; i++){
        lista_insertar_ultimo(lista, &arreglo[i]);
    }
    
    //  Al remover el elemento cuando se crea el iterador, cambia el primer elemento de la lista.
    lista_iter_t* iterador = lista_iter_crear(lista);
    int* dato1 = (int*)lista_ver_primero(lista);
    lista_iter_borrar(iterador);
    int* dato2 = (int*)lista_ver_primero(lista);
    print_test("Al remover el elemento cuando se crea el iterador, cambia el primer elemento de la lista", dato1 != dato2);
    lista_iter_destruir(iterador);
    lista_destruir(lista,NULL);
}

void iterador_insertar_remover_al_medio(int* arreglo,size_t tam_arreglo){
    lista_t* lista = lista_crear();
    int elem = 20;

    for(int i =0; i < tam_arreglo; i++){
         lista_insertar_ultimo(lista, &arreglo[i]);
    }

    lista_iter_t* iterador = lista_iter_crear(lista);

    for (int j = 0; j < (tam_arreglo/2); j++){
         lista_iter_avanzar(iterador);
    }
    int* anterior = (int*)lista_iter_ver_actual(iterador);
    lista_iter_insertar(iterador, &elem);
    int* actual = (int*)lista_iter_ver_actual(iterador);
    lista_iter_avanzar(iterador);
    print_test("Insertar un elemento en el medio se hace en la posición correcta", 
                actual == &elem
                && anterior == (int*)lista_iter_ver_actual(iterador));

    void* dato = lista_iter_borrar(iterador);
    lista_iter_destruir(iterador);

    iterador = lista_iter_crear(lista);
    bool tiene_dato= false;

    while (!lista_iter_al_final(iterador) && !tiene_dato){
        tiene_dato = lista_iter_ver_actual(iterador) == dato;
        lista_iter_avanzar(iterador);
    }

    print_test("Al remover un elemento del medio, este no está", !tiene_dato);

    lista_iter_destruir(iterador);
    lista_destruir(lista,NULL);
}

void iterador_insertar_remover_al_final(int* arreglo,size_t tam_arreglo){
    lista_t* lista = lista_crear();
    
    for(int i =0; i < tam_arreglo; i++){
         lista_insertar_ultimo(lista, &arreglo[i]);
    }
    lista_iter_t* iterador = lista_iter_crear(lista);
    while(!lista_iter_al_final(iterador)){
        lista_iter_avanzar(iterador);
    }
    lista_iter_insertar(iterador,&arreglo[0]);
    print_test("Insertar un elemento cuando el iterador está al final efectivamente es equivalente a insertar al final", 
                lista_ver_ultimo(lista) == lista_iter_ver_actual(iterador));

    void* ultimo = lista_ver_ultimo(lista);
    lista_iter_borrar(iterador);
    print_test("Remover el último elemento con el iterador cambia el último de la lista", ultimo != lista_ver_ultimo(lista));

    lista_iter_destruir(iterador);
    lista_destruir(lista,NULL);
}

void pruebas_iterador_externo(){
    printf("%s","PRUEBAS DE ITERADOR EXTERNO\n");
    int* arreglo = malloc(10 * sizeof(int));
    size_t tam_arreglo = 10;
	for(int i = 0; i < 10; i++) {
		arreglo[i] = i;
	};
    
    iterador_insertar_al_principio();
    iterador_remover_al_principio(arreglo,tam_arreglo);
    iterador_insertar_remover_al_medio(arreglo,tam_arreglo);
    iterador_insertar_remover_al_final(arreglo,tam_arreglo);
    free(arreglo); 
}

// Compara el orden de una lista (ascendente o descendente), respecto de un arreglo.
// Pre: La lista y el arreglo tienen la misma longitud.
bool comparador_orden(lista_t* lista, int* datos, bool ascendente){
    lista_iter_t* iterador = lista_iter_crear(lista);
    size_t i = ascendente? 0 : lista_largo(lista)-1;
    bool lista_esta_ordenada = true;
    while (!lista_iter_al_final(iterador) && lista_esta_ordenada){
        lista_esta_ordenada = *(int*)lista_iter_ver_actual(iterador) == datos[i];
        lista_iter_avanzar(iterador);
        ascendente? i++ : i--;
    }
    lista_iter_destruir(iterador);   
    return lista_esta_ordenada;
}

void pruebas_lista_vacia(){
    printf("%s","PRUEBAS DE LISTA VACIA\n");
    lista_t* lista = lista_crear();
    print_test("La lista se pudo crear", lista);
    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("Lista ver primero es NULL", !lista_ver_primero(lista));
    print_test("Lista ver ultimo es NULL", !lista_ver_ultimo(lista));
    lista_destruir(lista,NULL);
    printf("\n");
}

void pruebas_insercion(char* mensaje, bool (*insertar)(lista_t*,void*), bool orden_ascendente){
    printf("%s%s",mensaje,"\n");
    int datos[] = {1,2,3,4,5};
    size_t longitud_datos = 5;
    lista_t* lista = lista_crear();
    int i = 0;

    print_test("Se puede insertar un elemento",insertar(lista,&datos[0]));
    print_test("Se puede eliminar un elemento", lista_borrar_primero(lista)!= NULL);
    print_test("La lista  esta vacia", lista_esta_vacia(lista));

    while (i < longitud_datos && insertar(lista,&datos[i])){
        i++;
    }
    print_test("Se pudieron insertar varios los elementos", lista_largo(lista) == longitud_datos); 
    print_test("La lista ya no esta vacia",!lista_esta_vacia(lista));
    print_test("Los elementos se encuentran en el orden correcto", comparador_orden(lista,datos,orden_ascendente));

    lista_destruir(lista,NULL);
    printf("\n");
}

void pruebas_insertar_primero(){
    pruebas_insercion("PRUEBAS INSERTAR PRIMERO", lista_insertar_primero,false);
}

void pruebas_insertar_ultimo(){
    pruebas_insercion("PRUEBAS INSERTAR ULTIMO", lista_insertar_ultimo,true);
}

void pruebas_null(){
    printf("%s","PRUEBAS CON NULL\n");
    lista_t* lista = lista_crear();
    print_test("Se puede insertar NULL",lista_insertar_primero(lista,NULL));
    print_test("El primer elemento de la lista es NULL", !lista_ver_primero(lista));
    print_test("La lista no esta vacia", lista_largo(lista) != 0);
    print_test("Borrar el primer elemento devuelve NULL", !lista_borrar_primero(lista));
    print_test("La lista esta vacia.", lista_esta_vacia(lista));
    lista_destruir(lista,NULL);
    printf("\n");
}

void pruebas_volumen(){
    printf("%s","PRUEBAS DE VOLUMEN\n");
    lista_t* lista = lista_crear();
    size_t longitud = 10000;
    int *datos = malloc(longitud * sizeof(int));
    
    for (int i = 0; i < longitud; i++)
    {
        datos[i] = i;
    }

    size_t j = 0;
    while(j < longitud && lista_insertar_primero(lista,&datos[j])){
        j++;
    }

    print_test("Se pudieron insertar todos los elementos", lista_largo(lista) == longitud);
    print_test("La lista no esta vacia",!lista_esta_vacia(lista));

    while (!lista_esta_vacia(lista)){
        lista_borrar_primero(lista);
    }
    print_test("Se pudieron eliminar todos los elementos", lista_esta_vacia(lista));
    
    free(datos);
    lista_destruir(lista,NULL);
    printf("\n");
}

void pruebas_lista_estudiante(){
    pruebas_lista_vacia();
    pruebas_insertar_primero();
    pruebas_insertar_ultimo();
    pruebas_null();
    pruebas_volumen();
    pruebas_iterador_interno_con_corte();
    pruebas_iterador_interno_sin_corte();
    pruebas_iterador_externo();
}

#ifndef CORRECTOR
int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;
}
#endif


