#include "abb.h"
#include <stdlib.h>
#include <string.h>

typedef struct nodo_abb{
	struct nodo_abb* izq;
	struct nodo_abb* der;
	char* clave;
	void* dato;
}nodo_abb_t;

struct abb{
	nodo_abb_t* raiz;
	size_t cant;
	abb_destruir_dato_t destruir;
	abb_comparar_clave_t comparar;
};

/*---------------------------------------------
			PRIMITIVAS DEL ABB
-----------------------------------------------*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t));

	if (!abb) 
		return NULL;

	abb->raiz = NULL;
	abb->cant = 0;
	abb->destruir = destruir_dato;
	abb->comparar = cmp;

	return abb;
}

// devuelve un nodo vacio e inicializado, o NULL en caso
// de no poder reservar memoria
nodo_abb_t* nodo_crear( void ) {
	nodo_abb_t *nodo = malloc( sizeof(nodo_abb_t) );
	if( !nodo ) return NULL;

	nodo->clave = NULL;
	nodo->dato = NULL;
	nodo->der = NULL;
	nodo->izq = NULL;
	return nodo;
}

// funcion auxiliar recursiva para abb_buscar
nodo_abb_t* abb_buscar_rec( nodo_abb_t *raiz, const char *clave, abb_comparar_clave_t comparar ) {
	if( (raiz->der == NULL) && (raiz->izq == NULL) ) {
		return raiz;
	}

	int comparacion = comparar( clave, raiz->clave );
	nodo_abb_t * nodo_resultado;
	if( comparacion == 0 ) {
		nodo_resultado = raiz;
	}
	else if( (comparacion > 0) && (raiz->der != NULL) ) {
		nodo_resultado = abb_buscar_rec( raiz->der, clave, comparar );
	}
	else if( (comparacion < 0) && (raiz->izq != NULL) ) {
		nodo_resultado = abb_buscar_rec( raiz->izq, clave, comparar );
	}
	return nodo_resultado;
}

// devuelve el nodo donde se encuentra el par (clave, dato).
// En caso de que no esté en el ABB, se devuelve el nodo 
// que lo tendria como hijo si existiera.
// pre: se inicializó el abb y tiene al menos 1 elemento 
nodo_abb_t* abb_buscar( abb_t *abb, const char *clave ) {
	nodo_abb_t *nodo = abb_buscar_rec( abb->raiz, clave, abb->comparar );
	return nodo;
}

bool abb_guardar( abb_t *arbol, const char *clave, void *dato ) {
	nodo_abb_t *nodo;
	if( abb_cantidad(arbol) == 0 ) {
		nodo = nodo_crear();
		if( !nodo ) return false;
		arbol->raiz = nodo;
	}
	else {
		nodo_abb_t *nodo_aux = abb_buscar( arbol, clave );
		int comp = arbol->comparar( clave, nodo_aux->clave );
		if( comp == 0 ) { //hay que reemplazar
			free( nodo_aux->clave );
			arbol->destruir( nodo_aux->dato );
			nodo = nodo_aux;
		}
		else if( comp > 0 ) {
			nodo_aux->der = nodo_crear();
			if( !nodo_aux->der ) return false;
			nodo = nodo_aux->der;
		}
		else if( comp < 0 ) {
			nodo_aux->izq = nodo_crear();
			if( !nodo_aux->izq ) return false;
			nodo = nodo_aux->izq;
		}
	}
	nodo->clave = strdup(clave);
	nodo->dato = dato;
	return true;
}