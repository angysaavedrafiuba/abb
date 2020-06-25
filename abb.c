#include "abb.h"
//#include <stdlib.h>

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