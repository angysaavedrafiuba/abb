#include "abb.h"
#include <stdlib.h>

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