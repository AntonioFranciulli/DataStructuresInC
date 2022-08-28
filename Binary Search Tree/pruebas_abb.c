#include "testing.h"
#include "abb.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



/* ******************************************************************
 *                      FUNCIÓNES AUXILIARES
 * *****************************************************************/

bool printear(const char* clave,void* dato, void* extra){
    if(!dato) return false;
    printf("%s- %s\n",clave,(char*)dato);
    return true;
}

int comparador(const char* clave1,const char* clave2){
    return strcmp(clave1,clave2);
}

void swap(void* A[], int i, int j){
    void* temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}


// Función que desordena un arreglo de strings mediante el algoritmo Fisher–Yates (necesaría para las pruebas de volumen)
// Basada en esta implementación: https://www.techiedelight.com/shuffle-given-array-elements-fisher-yates-shuffle/
static void desordenar(void* claves, size_t largo){
    for(int i = (int) (largo - 1); i > 0; i--) {
        int j = rand() % (i+1);
        swap((void**) claves, i, j);
    }
}



/* ******************************************************************
 *                        PRUEBAS DE ABB
 * *****************************************************************/

void prueba_crear_abb_vacio(){
    printf("Pruebas con ABB Vacío:\n");
    abb_t* arbol = abb_crear(comparador, NULL);
    print_test("Se pudo crear el arbol vacío.", arbol);
    print_test("La cantidad de elementos en un ABB Vacío es 0.", abb_cantidad(arbol) == 0);
    print_test("Un elemento cualquiera no pertenece al ABB.", abb_pertenece(arbol, "A") == false);
    print_test("Obtener clave de un elemento cualquiera falla.", !abb_obtener(arbol, "A"));
    print_test("Borrar en el ABB vacío falla.", !abb_borrar(arbol, "A"));
    abb_destruir(arbol);
    printf("\n");
}

void prueba_iterar_abb_vacio(){
    printf("Pruebas Iterador Externo ABB Vacío:\n");
    abb_t* arbol = abb_crear(comparador, NULL);
    abb_iter_t* iter = abb_iter_in_crear(arbol);
    print_test("Se creo un iterador.", iter);
    print_test("El iterador está al final del ABB.", abb_iter_in_al_final(iter));
    print_test("No se pudo avanzar el iterador", abb_iter_in_avanzar(iter) == false);
    print_test("El elemento actual donde está el iterador es NULL", !abb_iter_in_ver_actual(iter));
    abb_iter_in_destruir(iter);
    abb_destruir(arbol);
    printf("\n");
}

void prueba_abb_guardar(){
    printf("Pruebas Guardar ABB:\n");
    char *clave1 = "1", *valor1 = "A";
    char *clave2 = "3", *valor2 = "B";
    char *clave3 = "4", *valor3 = "C";
    char *clave4 = "2", *valor4 = "D";
    char *clave5 = "5", *valor5 = "E";

    abb_t* arbol = abb_crear(comparador,NULL);
    print_test("Se pudo crear el arbol vacío", arbol);
    print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);
    print_test("Obtener la clave A es NULL", !abb_obtener(arbol,"A"));
    print_test("Se puede guardar clave1 y valor1", abb_guardar(arbol,clave1,valor1));
    print_test("Clave1 pertenece al ABB", abb_pertenece(arbol,clave1));
    print_test("Se puede guardar clave2 y valor2", abb_guardar(arbol,clave2,valor2));
    print_test("Clave2 pertenece al ABB", abb_pertenece(arbol,clave2));
    print_test("Se puede guardar clave3 y valor3", abb_guardar(arbol,clave3,valor3));
    print_test("Clave3 pertenece al ABB", abb_pertenece(arbol,clave3));
    print_test("Se puede guardar clave4 y valor4", abb_guardar(arbol,clave4,valor4));
    print_test("Clave4 pertenece al ABB", abb_pertenece(arbol,clave4));
    print_test("Se puede guardar clave5 y valor5", abb_guardar(arbol,clave5,valor5));
    print_test("Clave5 pertenece al ABB", abb_pertenece(arbol,clave5));

    print_test("Borrar elemento 1 devuelve valor1", strcmp((char*)abb_borrar(arbol, clave1),valor1) == 0);
    print_test("Borrar elemento 2  devuelve valor2", strcmp((char*)abb_borrar(arbol, clave2),valor2) == 0);
    print_test("Borrar elemento 4  devuelve valor4", strcmp((char*)abb_borrar(arbol, clave4),valor4) == 0);
    print_test("Borrar elemento 3  devuelve valor3", strcmp((char*)abb_borrar(arbol, clave3),valor3) == 0);
    print_test("Borrar elemento 5  devuelve valor5", strcmp((char*)abb_borrar(arbol, clave5),valor5) == 0);
    print_test("Intento borrar elemento 5 de nuevo y no puedo", !abb_borrar(arbol, clave5));
    print_test("Clave 5 no pertenece al abb", !abb_pertenece(arbol,clave5));
    print_test("ABB esta vacio", abb_cantidad(arbol)==0);
    abb_destruir(arbol);
    printf("\n");
}

void prueba_abb_casos_borde(){
    printf("Prueba casos borde en ABB (Clave y Valor vacío o NULL):\n");
    abb_t* arbol = abb_crear(comparador,NULL);
    char *clave = "", *valor = NULL;
    char *clave2 = "", *valor2 = "";

    print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);
    print_test("Se pudo insertar una clave vacía con valor NULL", abb_guardar(arbol, clave, valor));
    print_test("La cantidad de elementos es 1", abb_cantidad(arbol) == 1);
    print_test("Obtener la clave vacía nos devuelve NULL", abb_obtener(arbol, clave) == valor);
    print_test("La clave vacía pertenece al ABB", abb_pertenece(arbol, clave));
    print_test("Intentar borrar la clave vacía devuelve NULL", abb_borrar(arbol, clave) == valor);
    
    print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);
    print_test("Se pudo insertar clave vacía y un valor vacío", abb_guardar(arbol, clave2, valor2));
    print_test("La cantidad de elementos es 1", abb_cantidad(arbol) == 1);
    print_test("Obtener la clave vacía nos devuelve NULL", abb_obtener(arbol, clave2) == valor2);
    print_test("La clave vacía pertenece al ABB", abb_pertenece(arbol, clave2));
    print_test("Intentar borrar la clave vacía devuelve NULL", abb_borrar(arbol, clave2) == valor2);
    print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);

    abb_destruir(arbol);
    printf("\n");
}

void prueba_abb_iterar_pocos(){
    printf("Pruebas Iterador Externo Pocos elementos:\n");
    abb_t* arbol = abb_crear(comparador,NULL);
    print_test("Se pudo crear el arbol vacío", arbol);

    char *clave1 = "1", *valor1 = "A";
    char *clave2 = "2", *valor2 = "B";
    char *clave3 = "3", *valor3 = "C";
    char *clave4 = "4", *valor4 = "D";
    char *clave5 = "5", *valor5 = "E";
    
    print_test("Se guardó la clave3 '3' y valor 3", abb_guardar(arbol,clave3,valor3));
    print_test("Se guardó la clave2 '2' y valor 2", abb_guardar(arbol,clave2,valor2));
    print_test("Se guardó la clave4 '4' y valor 4", abb_guardar(arbol,clave4,valor4));
    print_test("Se guardó la clave5 '5' y valor 5", abb_guardar(arbol,clave5,valor5));
    print_test("Se guardó la clave1 '1' y valor 1", abb_guardar(arbol,clave1,valor1));

    abb_iter_t* iter = abb_iter_in_crear(arbol);
    print_test("Se creó un iterador", iter);
    print_test("El iterador no se encuentra al final del ABB", abb_iter_in_al_final(iter) == false);
    
    print_test("La clave del nodo donde se encuentra el iterador es la clave1", strcmp(abb_iter_in_ver_actual(iter),clave1) == 0);
    print_test("El iterador avanzó 1 elemento", abb_iter_in_avanzar(iter));
    print_test("La clave del nodo donde se encuentra el iterador es la clave2", strcmp(abb_iter_in_ver_actual(iter),clave2) == 0);
    print_test("El iterador avanzó 1 elemento", abb_iter_in_avanzar(iter));
    print_test("La clave del nodo donde se encuentra el iterador es la clave3", strcmp(abb_iter_in_ver_actual(iter),clave3) == 0);
    print_test("El iterador avanzó 1 elemento", abb_iter_in_avanzar(iter));
    print_test("La clave del nodo donde se encuentra el iterador es la clave4", strcmp(abb_iter_in_ver_actual(iter),clave4) == 0);
    print_test("El iterador avanzó 1 elemento", abb_iter_in_avanzar(iter));
    print_test("La clave del nodo donde se encuentra el iterador es la clave5", strcmp(abb_iter_in_ver_actual(iter),clave5) == 0);
    print_test("El iterador avanzó 1 elemento", abb_iter_in_avanzar(iter));

    print_test("El iterador esta al final", abb_iter_in_al_final(iter));
    abb_iter_in_destruir(iter);
    abb_destruir(arbol);
    printf("\n");
}

void prueba_abb_recorrer_in_order(){
    printf("Pruebas Iterador Interno InOrder ABB:\n");
    abb_t* arbol = abb_crear(comparador,NULL);
    print_test("Se pudo crear el arbol vacío", arbol);
    char *clave1 = "1", *valor1 = "A";
    char *clave2 = "2", *valor2 = "B";
    char *clave3 = "3", *valor3 = "C";
    char *clave4 = "4", *valor4 = "D";
    char *clave5 = "5", *valor5 = "E";
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave4,valor4));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave2,valor2));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave3,valor3));
    print_test("Se guardó un elemento", abb_guardar(arbol,clave1,valor1));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave5,valor5));
    abb_in_order(arbol, printear, NULL);
    abb_destruir(arbol);
    printf("\n");
}

void prueba_abb_borrar(){
    printf("Pruebas Borrar ABB:\n");
    abb_t* arbol = abb_crear(comparador,NULL);
    print_test("Se pudo crear el arbol vacío", arbol);
    char *clave1 = "10", *valor1 = "cuarentayuno";
    char *clave2 = "71", *valor2 = "diez";
    char *clave3 = "41", *valor3 = "setentayuno";
    char *clave4 = "91", *valor4 = "noventayuno";
    char *clave5 = "47", *valor5 = "cuarentaysiete";
    char *clave6 = "2", *valor6 = "uno";

    print_test("Se guardó un elemento", abb_guardar(arbol,clave1,valor1));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave2,valor2));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave3,valor3));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave4,valor4));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave5,valor5));
    print_test("Se puede guardar un elemento", abb_guardar(arbol,clave6,valor6));
    print_test("Se agregaron todos los elementos correctamente", abb_cantidad(arbol) == 6);

    print_test("Se elimino un nodo con 1 solo hijo", abb_borrar(arbol, clave3) == valor3);
    print_test("La cantidad de elementos es la correcta", abb_cantidad(arbol) == 5);
    print_test("Se verificó que su hijo sigue perteneciendo al ABB", abb_pertenece(arbol, clave5));
    print_test("El nodo borrado ya no esta en el ABB", abb_pertenece(arbol, clave3) == false);
    print_test("Si intento borrar el nodo borrado no puedo", !abb_borrar(arbol, clave3));

    print_test("Se elimino un nodo con 2 hijos", abb_borrar(arbol, clave2) == valor2);                   
    print_test("La cantidad de elementos es la correcta", abb_cantidad(arbol) == 4);
    print_test("Se verificó que sus hijos siguen perteneciendo al ABB", abb_pertenece(arbol, clave4) && abb_pertenece(arbol, clave5));
    print_test("El nodo borrado ya no esta en el ABB", abb_pertenece(arbol, clave2) == false);
    print_test("Si intento borrar el nodo borrado no puedo", !abb_borrar(arbol, clave2));

    abb_borrar(arbol, clave6);
    abb_borrar(arbol, clave1);
    abb_borrar(arbol, clave4);
    abb_borrar(arbol, clave5);

    print_test("Se eliminaron todos los nodos del ABB", abb_cantidad(arbol) == 0);
    print_test("Intento borrar nodos en un ABB vacío y no puedo", !abb_borrar(arbol, clave6));

    abb_destruir(arbol);
    printf("\n");
}

static void prueba_abb_volumen(size_t largo){
    printf("Pruebas ABB Volumen:\n");
    abb_t* arbol = abb_crear(comparador,NULL);
    print_test("Se pudo crear el arbol vacío", arbol);

    const size_t largo_clave = 10;
    char* claves[largo];
    unsigned* valores[largo];
    for (unsigned i = 0; i < largo; i++) {
        claves[i] = malloc(largo_clave);
        valores[i] = malloc(sizeof(unsigned));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }
    
    desordenar(claves, largo);
   
    bool ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(arbol, claves[i], valores[i]);
        if (!ok) break;
        ok = abb_pertenece(arbol, claves[i]);
        if (!ok) break;
        ok = abb_obtener(arbol, claves[i]) == valores[i];
        if (!ok) break;
    }

    print_test("Se guardaron correctamente muchos elementos", ok);
    print_test("Se verificó que cada elemento pertenece al ABB", ok);
    print_test("La cantidad de nodos es correcta", abb_cantidad(arbol) == largo);

    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(arbol, claves[i]) == valores[i];
        if (!ok) break;
        ok = abb_pertenece(arbol, claves[i]) == false;
        if (!ok) break;
    }

    print_test("Se borraron muchos elementos correctamente", ok);
    print_test("Se verificó que cada elemento eliminado ya no pertenece al ABB", ok);
    print_test("La cantidad de nodos es correcta", abb_cantidad(arbol) == 0);

    abb_destruir(arbol);
    arbol = abb_crear(comparador, free);
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(arbol, claves[i], valores[i]);
        if (!ok) break;
    }
    for(size_t i = 0; i < largo; i++) {
        free(claves[i]);
    }
    abb_destruir(arbol);
    printf("\n");
}

void prueba_abb_iterar_volumen(size_t largo){
    printf("Pruebas Iterar ABB Volumen:\n");
    abb_t* arbol = abb_crear(comparador,NULL);
    print_test("Se pudo crear el arbol vacío", arbol);

    const size_t largo_clave = 10;
    char* claves[largo];
    unsigned* valores[largo];

    for (unsigned i = 0; i < largo; i++) {
        claves[i] = malloc(largo_clave);
        valores[i] = malloc(sizeof(unsigned));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }
 
    desordenar(claves, largo);

    bool ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(arbol, claves[i], valores[i]);
        if (!ok) break;
        ok = abb_pertenece(arbol, claves[i]);
        if (!ok) break;
        ok = abb_obtener(arbol, claves[i]) == valores[i];
        if (!ok) break;
    }

    print_test("Se guardaron correctamente muchos elementos", ok);
    print_test("Se verificó que cada elemento pertenece al ABB", ok);
    print_test("La cantidad de nodos es correcta", abb_cantidad(arbol) == largo);

    abb_iter_t* iter = abb_iter_in_crear(arbol);
    print_test("Se creo un iterador", iter);
    print_test("El iterador no se encuentra al final del ABB", abb_iter_in_al_final(iter) == false);
    
    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if (abb_iter_in_al_final(iter)) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if (!clave) {
            ok = false;
            break;
        }
        valor = abb_obtener(arbol, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        valor = NULL;
        abb_iter_in_avanzar(iter);
    }
    print_test("El iterador iteró sobre todo el ABB con éxito", ok);
    print_test("El iterador está posicionado en el último nodo del ABB", abb_iter_in_al_final(iter) && (i == largo));
    abb_destruir(arbol);
    abb_iter_in_destruir(iter);
    
    for(size_t j = 0; j < largo; j++) {
        free(claves[j]);
        free(valores[j]);
    }
    printf("\n");
}



/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_abb_estudiante(){
    prueba_crear_abb_vacio();
    prueba_iterar_abb_vacio();
    prueba_abb_guardar();
    prueba_abb_casos_borde();
    prueba_abb_iterar_pocos();
    prueba_abb_recorrer_in_order();
    prueba_abb_borrar();
    prueba_abb_volumen(5000);
    prueba_abb_iterar_volumen(5000);
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif