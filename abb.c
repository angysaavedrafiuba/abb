#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
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

struct abb_iter {
	pila_t* pila;
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
	abb_destruir_rec(arbol->raiz, arbol->destruir);
	free(arbol);
}

bool abb_in_order_rec(nodo_abb_t* raiz, bool visitar(const char *, void *, void *), void *extra){
	if (!raiz) return true;
	if (!abb_in_order_rec(raiz->izq, visitar, extra))
		return false;
	if (!visitar(raiz->clave, raiz->dato, extra))
		return false;
	if (!abb_in_order_rec(raiz->der, visitar, extra))
		return false;
	return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	abb_in_order_rec(arbol->raiz, visitar, extra);
}

bool apilar_rama_izq(nodo_abb_t* nodo, pila_t* pila){
	if (!nodo) return true;

	if (!pila_apilar(pila, nodo)) return false;

	return apilar_rama_izq(nodo->izq, pila);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* abb_iter = malloc(sizeof(abb_iter_t));

	if(!abb_iter) return NULL;

	abb_iter->pila = pila_crear();

	if(!abb_iter->pila){
		free(abb_iter);
		return NULL;
	}

	if(!apilar_rama_izq(arbol->raiz, abb_iter->pila)){
		pila_destruir(abb_iter->pila);
		free(abb_iter);
		return NULL;
	}

	return abb_iter;
}


bool abb_iter_in_avanzar(abb_iter_t *iter){
	nodo_abb_t* actual = pila_desapilar(iter->pila);

	if(!actual) return false;

	if(actual->der) 
		return apilar_rama_izq(actual->der, iter->pila);

	return true;
}


const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	return ((nodo_abb_t*)pila_ver_tope(iter->pila))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}