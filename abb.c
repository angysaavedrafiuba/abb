#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo_abb {
  struct nodo_abb *izq;
  struct nodo_abb *der;
  char *clave;
  void *dato;
} nodo_abb_t;

struct abb {
  nodo_abb_t *raiz;
  size_t cantidad;
  abb_destruir_dato_t destruir;
  abb_comparar_clave_t comparar;
};

struct abb_iter {
  pila_t *pila;
};

/*---------------------------------------------
                        PRIMITIVAS DEL ABB
-----------------------------------------------*/

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
  abb_t *abb = malloc(sizeof(abb_t));

  if (!abb)
    return NULL;

  abb->raiz = NULL;
  abb->cantidad = 0;
  abb->destruir = destruir_dato;
  abb->comparar = cmp;

  return abb;
}

size_t abb_cantidad(abb_t *arbol) {
  return arbol->cantidad;
}

nodo_abb_t *abb_buscar(nodo_abb_t *nodo, const char *clave,
                       abb_comparar_clave_t comparar, nodo_abb_t **padre) {
  if( !nodo ) return NULL;

  int comparacion = comparar(clave, nodo->clave);
  nodo_abb_t *nodo_resultado = NULL;

  if (comparacion == 0) {
    nodo_resultado = nodo;
  } else {
    *padre = nodo;

    if (comparacion > 0) {
      nodo_resultado = abb_buscar(nodo->der, clave, comparar, padre);
    } else if (comparacion < 0) {
      nodo_resultado = abb_buscar(nodo->izq, clave, comparar, padre);
    }
  }

  return nodo_resultado;
}

// crea, si es posible, un nodo que contiene 
// la clave y el dato pasados por parametro
// en caso de no poder crearse, devuelve NULL
nodo_abb_t* nodo_crear(const char* clave, void* dato) {
  nodo_abb_t* nodo = calloc(1, sizeof(nodo_abb_t));
  if(!nodo) return NULL;

  nodo->clave = strdup(clave);
  nodo->dato = dato;
  return nodo;
}

// libera el espacio pedido para el nodo
// devuelve el dato contenido en el nodo
// pre: el nodo fue inicializado
void *nodo_borrar(nodo_abb_t *nodo) {
  void *dato_aux = nodo->dato;
  free(nodo->clave);
  free(nodo);
  return dato_aux;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
  nodo_abb_t* padre = NULL;
  nodo_abb_t *nodo_aux =
    abb_buscar(arbol->raiz, clave, arbol->comparar, &padre);
  
  if(nodo_aux) {
	  void* dato_viejo = nodo_aux->dato;
	  if(arbol->destruir) {
		  arbol->destruir(dato_viejo);
	  }
    nodo_aux->dato = dato;
    return true;
  }
  
  nodo_abb_t* nodo_nuevo = nodo_crear(clave, dato);
  if(padre) {
	  int comp = arbol->comparar(clave, padre->clave);
	  if(comp > 0)
	  	padre->der = nodo_nuevo;
	  else if(comp < 0)
	    padre->izq = nodo_nuevo;
  }
  else {
	  arbol->raiz = nodo_nuevo;
  }

  arbol->cantidad++;
  return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
  nodo_abb_t* padre = NULL;
  nodo_abb_t *nodo = abb_buscar(arbol->raiz, clave, arbol->comparar, &padre);

  if (!nodo)
    return NULL;

  return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
   nodo_abb_t* padre = NULL;
  if (!abb_buscar(arbol->raiz, clave, arbol->comparar, &padre))
    return false;

  return true;
}

void abb_destruir_rec(nodo_abb_t *raiz, abb_destruir_dato_t destruir) {
  if (!raiz)
    return;

  abb_destruir_rec(raiz->izq, destruir);
  abb_destruir_rec(raiz->der, destruir);

  free(raiz->clave);
  if (destruir)
    destruir(raiz->dato);
  free(raiz);
}

void abb_destruir(abb_t *arbol) {
  abb_destruir_rec(arbol->raiz, arbol->destruir);
  free(arbol);
}

// ****************** ABB BORRAR ******************

// primitiva auxiliar para abb_borrar
// borra un elemento en caso de que el nodo tenga un solo hijo
void *borrar_con_menos_de_dos_hijos(nodo_abb_t *nodo, nodo_abb_t *padre, abb_t *arbol) {
  nodo_abb_t *hijo = NULL;
  if (nodo->der) {
    hijo = nodo->der;
  } else {
    hijo = nodo->izq;
  }

  if (!padre) { // el nodo a borrar es raiz
    arbol->raiz = hijo;
  } else {
    if (padre->der == nodo) {
      padre->der = hijo;
    } else {
      padre->izq = hijo;
    }
  }
  return nodo_borrar(nodo);
}

// primitiva auxiliar para borrar_con_dos_hijos
// recibe el hijo derecho del nodo a reemplazar
// devuelve el nodo que debe reemplazar al nodo pasado por
// parametro y actualiza la referencia al padre de este reemplazo
// como convencion, es el mas izquierdo de los derechos
nodo_abb_t *buscar_reemplazo(nodo_abb_t *nodo, nodo_abb_t **padre_reemplazo) {
  if (nodo->izq == NULL) {
    return nodo;
  }
  *padre_reemplazo = nodo;
  return buscar_reemplazo(nodo->izq, padre_reemplazo);
}

// primitiva auxiliar para abb_borrar
// devuelve la cantidad de hijos que tiene el nodo
// pasado por parametro
size_t ver_cant_hijos(nodo_abb_t *nodo) {
  size_t contador = 0;
  if (nodo->der)
    contador++;
  if (nodo->izq)
    contador++;
  return contador;
}

// primitiva auxiliar para abb_borrar
// borra un elemento en caso de que el nodo tenga dos hijos
void *borrar_con_dos_hijos(nodo_abb_t *nodo, abb_t *arbol) {
  nodo_abb_t *padre_reemplazo = nodo;

  nodo_abb_t *reemplazo = buscar_reemplazo(nodo->der, &padre_reemplazo);
  char *clave_reemplazo = strdup(reemplazo->clave);
  void *dato_reemplazo;

  dato_reemplazo = borrar_con_menos_de_dos_hijos(reemplazo, padre_reemplazo, arbol);

  free(nodo->clave);
  void *dato_a_devolver = nodo->dato;
  nodo->clave = clave_reemplazo;
  nodo->dato = dato_reemplazo;
  return dato_a_devolver;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
  if (!arbol->raiz)
    return NULL;

  nodo_abb_t *padre = NULL;
  nodo_abb_t *nodo_a_borrar =
      abb_buscar(arbol->raiz, clave, arbol->comparar, &padre);
  if (!nodo_a_borrar)
    return NULL;

  size_t cant_hijos = ver_cant_hijos(nodo_a_borrar);
  void *dato = NULL; // dato a devolver

  if (cant_hijos < 2) {
    dato = borrar_con_menos_de_dos_hijos(nodo_a_borrar, padre, arbol);
  } else {
    dato = borrar_con_dos_hijos(nodo_a_borrar, arbol);
  }

  arbol->cantidad--;
  return dato;
}

/*---------------------------------------------
                PRIMITIVAS DEL ITERADOR INTERNO
-----------------------------------------------*/

bool abb_in_order_rec(nodo_abb_t *raiz,
                      bool visitar(const char *, void *, void *), void *extra) {
  if (!raiz)
    return true;
  if (!abb_in_order_rec(raiz->izq, visitar, extra))
    return false;
  if (!visitar(raiz->clave, raiz->dato, extra))
    return false;
  return abb_in_order_rec(raiz->der, visitar, extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *),
                  void *extra) {
  abb_in_order_rec(arbol->raiz, visitar, extra);
}

/*---------------------------------------------
                PRIMITIVAS DEL ITERADOR EXTERNO
-----------------------------------------------*/

bool apilar_rama_izq(nodo_abb_t *nodo, pila_t *pila) {
  if (!nodo)
    return true;

  if (!pila_apilar(pila, nodo))
    return false;

  return apilar_rama_izq(nodo->izq, pila);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
  abb_iter_t *abb_iter = malloc(sizeof(abb_iter_t));

  if (!abb_iter)
    return NULL;

  abb_iter->pila = pila_crear();

  if (!abb_iter->pila) {
    free(abb_iter);
    return NULL;
  }

  if (!apilar_rama_izq(arbol->raiz, abb_iter->pila)) {
    pila_destruir(abb_iter->pila);
    free(abb_iter);
    return NULL;
  }

  return abb_iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
  nodo_abb_t *actual = pila_desapilar(iter->pila);

  if (!actual)
    return false;

  if (actual->der)
    return apilar_rama_izq(actual->der, iter->pila);

  return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
  nodo_abb_t* tope = pila_ver_tope(iter->pila);
  
  if(!tope)
    return NULL;

  return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
  return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t *iter) {
  pila_destruir(iter->pila);
  free(iter);
}