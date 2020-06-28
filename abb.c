#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct nodo_abb{
	struct nodo_abb* izq;
	struct nodo_abb* der;
	char* clave;
	void* dato;
}nodo_abb_t;

struct abb{
	nodo_abb_t* raiz;
	size_t cantidad;
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
	abb->cantidad = 0;
	abb->destruir = destruir_dato;
	abb->comparar = cmp;

	return abb;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}

nodo_abb_t* abb_buscar( nodo_abb_t *raiz, const char *clave, abb_comparar_clave_t comparar, nodo_abb_t** padre) {
	*padre = raiz;
	int comparacion = comparar( clave, raiz->clave );
	nodo_abb_t* nodo_resultado = NULL;

	if( comparacion == 0 ) {
		nodo_resultado = raiz;
	}
	else if( (comparacion > 0) && (raiz->der != NULL) ) {
		nodo_resultado = abb_buscar( raiz->der, clave, comparar, padre );
	}
	else if( (comparacion < 0) && (raiz->izq != NULL) ) {
		nodo_resultado = abb_buscar( raiz->izq, clave, comparar, padre );
	}
	return nodo_resultado;
}

bool abb_guardar( abb_t *arbol, const char *clave, void *dato ) {
	nodo_abb_t *nodo;
	if( abb_cantidad(arbol) == 0 ) {
		nodo = calloc(1, sizeof(nodo_abb_t));
		if( !nodo ) return false;
		arbol->raiz = nodo;
		arbol->cantidad++;
	}
	else {
		nodo_abb_t *padre = NULL;
		nodo_abb_t *nodo_aux = abb_buscar(arbol->raiz, clave, arbol->comparar, &padre);

		if(nodo_aux) {
			free( nodo_aux->clave );
			arbol->destruir( nodo_aux->dato );
			nodo = nodo_aux;
		}
		else{
			nodo = calloc(1, sizeof(nodo_abb_t));
			if( !nodo ) return false;

			int comp = arbol->comparar(clave, padre->clave);
			if( comp > 0 )
				padre->der = nodo;
			else if( comp < 0 )
				padre->izq = nodo;

			arbol->cantidad++;
		} 
	}
	nodo->clave = strdup(clave);
	nodo->dato = dato;
	return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_abb_t* nodo = abb_buscar(arbol->raiz, clave, arbol->comparar, NULL);

	if (!nodo)
		return NULL;

	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if (!abb_buscar(arbol->raiz, clave, arbol->comparar, NULL))
		return false;

	return true; 
}

void abb_destruir_rec(nodo_abb_t* raiz, abb_destruir_dato_t destruir){
	if(!raiz) return;

	abb_destruir_rec(raiz->izq, destruir);
	abb_destruir_rec(raiz->der, destruir);	

	free(raiz->clave);
	destruir(raiz->dato);
	free(raiz);
}

void abb_destruir(abb_t *arbol){
	if (arbol->cantidad > 0)
		abb_destruir_rec(arbol->raiz, arbol->destruir);
	free(arbol);
}


