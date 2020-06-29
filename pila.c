#include "pila.h"
#include <stdlib.h>

#define TAM_INICIAL 4
#define REDIM_UP 2
#define REDIM_DOWN 4

struct pila {
    void** datos;
    size_t cantidad;  
    size_t capacidad;  
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){
	pila_t* pila = malloc( sizeof(pila_t) );

    if (pila == NULL) 
        return NULL;
    
    pila->datos = malloc( TAM_INICIAL * sizeof(void*) );

    if (pila->datos == NULL){
        free(pila);
        return NULL;
    }

    pila->capacidad = TAM_INICIAL;
    pila->cantidad = 0;

    return pila;
}

void pila_destruir(pila_t* pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t* pila){
	return pila->cantidad == 0;
}

bool redimensionar(pila_t* pila, size_t proporcion){ 
	void** datos_aux = realloc( pila->datos, proporcion * sizeof(void*) );

	if (datos_aux == NULL)
		return false;

	pila->datos = datos_aux;
    pila->capacidad = proporcion;
    return true;
}

bool pila_apilar(pila_t *pila, void* valor){
	if (pila->capacidad == pila->cantidad){
		if ( !redimensionar(pila, pila->capacidad * REDIM_UP) )
			return false;
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;

	return true;
}

void* pila_ver_tope(const pila_t *pila){
	if (pila->cantidad > 0)
		return pila->datos[pila->cantidad - 1];
	
	return NULL;
}

void* pila_desapilar(pila_t *pila){
	if (pila->cantidad > 0 ){
		void* dato_desapilado = pila->datos[pila->cantidad - 1];
		pila->cantidad--;

		if (pila->cantidad <= (pila->capacidad / REDIM_DOWN) && pila->capacidad > TAM_INICIAL)
			redimensionar(pila, pila->capacidad / REDIM_DOWN);

		return dato_desapilado;
	}

	return NULL;
}