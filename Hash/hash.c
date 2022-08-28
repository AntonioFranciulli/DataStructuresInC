#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "pool_hashes.h"
#define LARGO_INICIAL 499
#define CANT_HASHES 4
#define FACTOR_AGRANDAMIENTO 7
#define FACTOR_REDUCCION 0.25f
#define FACTOR_OCUPACION 0.1f



/* Definición de tipos de datos */

typedef struct celda{
    char* clave;
    void* dato;
    size_t hash_nro;
} celda_t;

struct hash{
    float carga;
    celda_t* arreglo;
    size_t largo_arreglo;
    size_t inserciones;
    const char* primer_clave;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
    size_t posicion;
    const hash_t* hash;
};



/* Funciones Auxiliares */

// Devuelve la posicion de la clave de acuerdo al hash elegido.
size_t obtener_posicion(const char* clave, size_t largo_arreglo, size_t hash_nro){
    size_t posicion = 0;
    unsigned int largo = (unsigned int) strlen(clave);
    switch (hash_nro){
    case 0:
        posicion = APHash(clave,largo);
        break;    
    case 1:
        posicion = DEKHash(clave,largo);
        break;
    case 2:
        posicion = RSHash(clave,largo);
        break;
    case 3:
        posicion = PJWHash(clave,largo);
        break;    
    }
    return posicion % largo_arreglo;
}

//  Función que establece las condiciones iniciales del arreglo.
void inicializar_arreglo(hash_t* hash){
    for (size_t i = 0; i < hash->largo_arreglo; i++){   
        hash->arreglo[i].clave = NULL;
        hash->arreglo[i].dato = NULL;
        hash->arreglo[i].hash_nro = 0;
    }
}

// Busca una clave en la tabla.
// Si la encuentra devuelve la posicion. Devuelve -1 en caso contrario.
size_t buscar_clave(const hash_t* hash, const char* clave){
    for (size_t i = 0; i < CANT_HASHES; i++){
        size_t pos = obtener_posicion(clave,hash->largo_arreglo,i);
        if(hash->arreglo[pos].clave && clave){
            if(strcmp(clave,hash->arreglo[pos].clave) == 0){
                return pos;
            }
        } 
    }
    return ((size_t)-1);
}

void redimensionar(hash_t* hash,float factor);   //  Declaración de función redimensionar que se utiliza en guardar.

//  Función guardar que con los criterios de Cuckoo hashing intenta guardar en la posición deseada.
void guardar(hash_t* hash, const char* clave, void* dato, size_t hash_nro,size_t llamadas){
    //Me fijo si la clave ya esta
    if(clave&&hash->primer_clave)
    if((strcmp(clave,hash->primer_clave)==0 && hash_nro == CANT_HASHES)|| llamadas > 25){
        redimensionar(hash,FACTOR_AGRANDAMIENTO);
    } 
    for (size_t i = 0; i < CANT_HASHES; i++){
        size_t pos = obtener_posicion(clave,hash->largo_arreglo,i);
        if(hash->arreglo[pos].clave && clave){
            if(strcmp(clave,hash->arreglo[pos].clave) == 0){
                if(hash->destruir_dato) hash->destruir_dato(hash->arreglo[pos].dato);
                hash->arreglo[pos].dato = dato;
                return;
            }
        } 
    }
    //Si la clave no esta, inserto en el primer lugar.
    size_t posicion = obtener_posicion(clave,hash->largo_arreglo,hash_nro%CANT_HASHES);
    char* clave_anterior = hash->arreglo[posicion].clave;
    void* dato_anterior = hash->arreglo[posicion].dato;
    size_t hash_anterior = hash->arreglo[posicion].hash_nro;
    hash->arreglo[posicion].clave = strdup(clave);
    hash->arreglo[posicion].dato = dato;
    hash->arreglo[posicion].hash_nro = hash_nro%CANT_HASHES;

    if(clave_anterior){
        hash_anterior++;
        guardar(hash,clave_anterior,dato_anterior,hash_anterior,++llamadas);
        free(clave_anterior);
    }else{
        hash->inserciones++;
    }
}

//  Función redimensionar que aumenta el largo del arreglo.
void redimensionar(hash_t* hash, float factor){
    celda_t* anterior = hash->arreglo;
    size_t largo_anterior = hash->largo_arreglo;
    size_t nuevo_largo = (size_t)((float)largo_anterior * factor);
    if (nuevo_largo < LARGO_INICIAL) nuevo_largo = LARGO_INICIAL;
    hash->arreglo = malloc(sizeof(celda_t)*nuevo_largo);
    if(!hash->arreglo) return;
    
    hash->largo_arreglo = (size_t)((float)largo_anterior*factor);
    hash->inserciones = 0;
    inicializar_arreglo(hash);
    for (size_t i = 0; i < largo_anterior; i++){
        if(anterior[i].clave){
            guardar(hash,anterior[i].clave,anterior[i].dato,0,0);
            free(anterior[i].clave);
        } 
    }
    free(anterior);
}



/* Primitivas del Hash  */

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    hash->arreglo = malloc(sizeof(celda_t)*LARGO_INICIAL);
    if(!hash->arreglo){
        free(hash);
        return NULL;
    }
    hash->carga = 0;
    hash->destruir_dato = destruir_dato;
    hash->largo_arreglo = LARGO_INICIAL;
    inicializar_arreglo(hash);
    hash->inserciones = 0;
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    float ocupacion = ((float)hash->inserciones/(float)hash->largo_arreglo);
    if(ocupacion< FACTOR_OCUPACION && hash->largo_arreglo>LARGO_INICIAL) redimensionar(hash,FACTOR_REDUCCION);
    hash->primer_clave = clave;
    size_t llamadas = 0;
    guardar(hash,clave,dato,0,llamadas);
    hash->carga = (float) hash->inserciones / (float) hash->largo_arreglo;
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    for (size_t i = 0; i < CANT_HASHES; i++){
    size_t pos = obtener_posicion(clave,hash->largo_arreglo,i);
    if(hash->arreglo[pos].clave && clave){
        if(strcmp(clave,hash->arreglo[pos].clave) == 0){
            void* dato = hash->arreglo[pos].dato;
            free(hash->arreglo[pos].clave);
            hash->arreglo[pos].clave = NULL;
            hash->arreglo[pos].dato = NULL;
            hash->arreglo[pos].hash_nro = 0;
            hash->inserciones--;
            return dato;
            }
        } 
    }
    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    size_t posicion = buscar_clave(hash,clave);
    return posicion == -1 ? NULL : hash->arreglo[posicion].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t posicion = buscar_clave(hash,clave);
    return posicion != -1;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->inserciones;
}

void hash_destruir(hash_t *hash){
    for (size_t i = 0; i < hash->largo_arreglo; i++){
        if(hash->arreglo[i].clave)
        if (hash->destruir_dato){
            hash->destruir_dato(hash->arreglo[i].dato);
        } 
        free(hash->arreglo[i].clave);
    }
    free(hash->arreglo);
    free(hash);
}



/* Primitivas del iterador*/

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if(!iter) return NULL;
    iter->hash = hash;
    for (iter->posicion = 0; iter->posicion < hash->largo_arreglo; iter->posicion++){
        if(hash->arreglo[iter->posicion].clave) break;
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if(hash_iter_al_final(iter)) return false;
    iter->posicion++;
    while(iter->posicion< iter->hash->largo_arreglo){
        if(iter->hash->arreglo[iter->posicion].clave) break;
        iter->posicion++;
    }
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    return hash_iter_al_final(iter) ? NULL : iter->hash->arreglo[iter->posicion].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return iter->posicion == iter->hash->largo_arreglo;
}

void hash_iter_destruir(hash_iter_t* iter){
    free(iter);
}
