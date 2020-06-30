#include "abb.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void abb_destruir_aux(void *abb) { abb_destruir(abb); }

void prueba_crear_abb_destruir_free() {
  printf("------Inicio pruebas crear abb con free\n");
  abb_t *ejemplo = abb_crear(strcmp, free);
  print_test("abb creado", ejemplo != NULL);
  abb_destruir(ejemplo);
}

void prueba_crear_abb_destruir_funcion() {
  printf("------Inicio pruebas crear abb con función destruir\n");
  abb_t *ejemplo = abb_crear(strcmp, abb_destruir_aux);
  print_test("abb creado", ejemplo != NULL);
  abb_destruir(ejemplo);
}

void prueba_crear_abb_destruir_NULL() {
  printf("------Inicio pruebas crear abb con NULL\n");
  abb_t *ejemplo = abb_crear(strcmp, NULL);
  print_test("abb creado", ejemplo != NULL);
  abb_destruir(ejemplo);
}

void prueba_destruir_abb_vacio_con_free() {
  printf("------Inicio pruebas destruir abb vacío con free\n");
  abb_t *ejemplo = abb_crear(strcmp, free);
  print_test("Abb tiene cantidad 0", !abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}

void prueba_destruir_abb_vacio_con_funcion() {
  printf("------Inicio pruebas destruir abb vacío con función\n");
  abb_t *ejemplo = abb_crear(strcmp, abb_destruir_aux);
  print_test("Abb tiene cantidad 0", !abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}

void prueba_destruir_abb_vacio_con_NULL() {
  printf("------Inicio pruebas destruir abb vacío con NULL\n");
  abb_t *ejemplo = abb_crear(strcmp, NULL);
  print_test("Abb tiene cantidad 0", !abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}

void prueba_abb_esta_vacio(){
  printf("------Inicio pruebas abb vacío\n");
  abb_t *ejemplo = abb_crear(strcmp, free);
  print_test("Abb tiene cantidad 0", !abb_cantidad(ejemplo));
  print_test("Obtener clave A, es NULL, no existe", !abb_obtener(ejemplo, "A"));
  print_test("Pertenece clave A, es false, no existe", !abb_pertenece(ejemplo, "A"));
  print_test("Borrar clave A, es NULL, no existe", !abb_borrar(ejemplo, "A"));
  abb_destruir(ejemplo);
}

static void prueba_iterar_abb_vacio(){
    printf("------Inicio pruebas iterar abb vacío\n");
    abb_t* abb = abb_crear(NULL, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

void prueba_destruir_abb_no_vacio_con_free() {
  printf("------Inicio pruebas destruir abb no vacío con free\n");
  abb_t *ejemplo = abb_crear(strcmp, free);
  void *valores[10];
  char str[2];

  for (int i = 0; i < 10; i++) {
    valores[i] = malloc(sizeof(char));
    sprintf(str, "%d", rand() % 10);
    abb_guardar(ejemplo, str, valores[i]);
  }

  print_test("abb después de insertar valores NO está vacío",
             abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}

void prueba_destruir_abb_no_vacio_con_funcion() {
  printf("------Inicio pruebas destruir abb no vacío con función\n");
  abb_t *ejemplo = abb_crear(strcmp, abb_destruir_aux);
  void *valores[10];
  char str[2];

  for (int i = 0; i < 10; i++) {
    valores[i] = abb_crear(strcmp, free);
    sprintf(str, "%d", rand() % 10);
    abb_guardar(ejemplo, str, valores[i]);
  }

  print_test("abb después de insertar valores NO está vacío",
             abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}

void prueba_abb_borrar_un_elemento() {
  printf("------Inicio pruebas borrar un elemento y NULL\n");
  abb_t *ejemplo = abb_crear(strcmp, NULL);
  char *str = "str";
  int a = 1;

  print_test("prueba borrar con arbol vacio",
             abb_borrar(ejemplo, "sata") == NULL);
  print_test("ingresando un elemento al arbol", abb_guardar(ejemplo, str, &a));
  print_test("la cantidad ahora es 1", abb_cantidad(ejemplo) == 1);
  print_test("prueba borrar con una clave que no está",
             abb_borrar(ejemplo, "ssd") == NULL);
  print_test("la cantidad sigue siendo 1", abb_cantidad(ejemplo) == 1);
  print_test("prueba borrar unico elemento", abb_borrar(ejemplo, str) == &a);
  print_test("la cantidad ahora es 0", abb_cantidad(ejemplo) == 0);
  print_test("ingresando un dato nulo", abb_guardar(ejemplo, str, NULL));
  print_test("la cantidad es la esperada", abb_cantidad(ejemplo) == 1);
  print_test("borrar devuelve el dato nulo", abb_borrar(ejemplo, str) == NULL);
  print_test("la cantidad vuelve a ser 0", abb_cantidad(ejemplo) == 0);
  abb_destruir(ejemplo);
}

void prueba_abb_borrar_varios_elementos() {
  printf("------Inicio pruebas borrar muchos elementos\n");
  abb_t *ejemplo = abb_crear(strcmp, NULL);
  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  char *a = "a";
  char *b = "b";
  char *c = "c";
  char *d = "d";
  char *e = "e";
  char *f = "f";
  char *g = "g";
  char *h = "h";
  char *i = "i";
  char *j = "j";
  abb_guardar(ejemplo, d, &arr[0]);
  abb_guardar(ejemplo, b, &arr[1]);
  abb_guardar(ejemplo, h, &arr[2]);
  abb_guardar(ejemplo, a, &arr[3]);
  abb_guardar(ejemplo, c, &arr[4]);
  abb_guardar(ejemplo, f, &arr[5]);
  abb_guardar(ejemplo, i, &arr[6]);
  abb_guardar(ejemplo, e, &arr[7]);
  abb_guardar(ejemplo, g, &arr[8]);
  abb_guardar(ejemplo, j, &arr[9]);
  print_test("se armó un arbol para las pruebas", true);
  print_test("la cantidad es la esperada", abb_cantidad(ejemplo) == 10);
  print_test("prueba borrar un nodo sin hijos",
             abb_borrar(ejemplo, a) == &arr[3]);
  print_test("la cantidad es la esperada", abb_cantidad(ejemplo) == 9);
  print_test("prueba borrar un nodo con hijo derecho",
             abb_borrar(ejemplo, b) == &arr[1]);
  print_test("la cantidad es la esperada", abb_cantidad(ejemplo) == 8);
  print_test("prueba borrar un nodo con dos hijos1",
             abb_borrar(ejemplo, f) == &arr[5]);
  print_test("la cantidad es la esperada", abb_cantidad(ejemplo) == 7);
  print_test("prueba borrar un nodo con dos hijos2",
             abb_borrar(ejemplo, h) == &arr[2]);
  print_test("la cantidad es la esperada", abb_cantidad(ejemplo) == 6);
  print_test("prueba borrar un nodo con hijo izquierdo",
             abb_borrar(ejemplo, g) == &arr[8]);
  print_test("la cantidad es la esperada", abb_cantidad(ejemplo) == 5);
  abb_destruir(ejemplo);
}

// esta prueba creo que deberiamos cambiarla antes de entregar, pero la dejo
// mientras porque para probar es útil
bool imprimir(const char *clave, void *dato, void *extra) {
  printf("clave: |%s|\n", clave);
  return true;
}

void prueba_iter_interno_sin_corte() {
  printf("------INICIO PRUEBAS ITERADOR INTERNO SIN CORTE\n");

  abb_t *ejemplo = abb_crear(strcmp, free);
  void *valores[10];
  char str[2];

  for (int i = 0; i < 10; i++) {
    valores[i] = malloc(sizeof(char));
    sprintf(str, "%d", rand() % 10);
    abb_guardar(ejemplo, str, valores[i]);
  }

  abb_in_order(ejemplo, imprimir, NULL);
  print_test("Itera con iterador interno todos los elementos", true);
  abb_destruir(ejemplo);
}

bool imprimir_top_3(const char *clave, void *dato, void *extra) {
  int *contador = extra;
  printf("%d. %s\n", ++(*contador), clave);
  return *contador < 3;
}

void prueba_iter_interno_con_corte() {
  printf("------INICIO PRUEBAS ITERADOR INTERNO CON CORTE\n");

  abb_t *ejemplo = abb_crear(strcmp, free);
  int contador = 0;
  void *valores[10];
  char str[2];

  for (int i = 0; i < 10; i++) {
    valores[i] = malloc(sizeof(char));
    sprintf(str, "%d", rand() % 10);
    abb_guardar(ejemplo, str, valores[i]);
  }

  abb_in_order(ejemplo, imprimir_top_3, &contador);
  print_test("Itera con condicion de corte ", contador == 3);
  abb_destruir(ejemplo);
}

void prueba_iter_externo() {
  printf("------INICIO PRUEBAS ITERADOR EXTERNO\n");
  abb_t *ejemplo = abb_crear(strcmp, abb_destruir_aux);
  void *valores[10];
  char str[2];

  for (int i = 0; i < 10; i++) {
    valores[i] = abb_crear(strcmp, free);
    sprintf(str, "%d", rand() % 10);
    abb_guardar(ejemplo, str, valores[i]);
  }

  abb_iter_t *abb_iter = abb_iter_in_crear(ejemplo);

  while (!abb_iter_in_al_final(abb_iter)) {
    const char *clave = abb_iter_in_ver_actual(abb_iter);
    printf("Actual clave %s\n", clave);
    abb_iter_in_avanzar(abb_iter);
  }

  abb_iter_in_destruir(abb_iter);

  abb_destruir(ejemplo);
}
/*
void prueba_pertenece_abb_no_vacio() {
  printf("------Inicio pruebas pertenece abb no vacío\n");
  abb_t *ejemplo = abb_crear(strcmp, abb_destruir_aux);
  void *valores[10];
  char str[2];

  for (int i = 0; i < 10; i++) {
    valores[i] = abb_crear(strcmp, free);
    sprintf(str, "%d", rand() % 10);
    abb_guardar(ejemplo, str, valores[i]);
  }

  print_test("abb después de insertar valores NO está vacío",
             abb_cantidad(ejemplo));
  sprintf(str, "%d", 1);
printf("obtener : %p", abb_obtener(ejemplo, str) );
//printf("obtener : %p", abb_obtener(ejemplo, 2) );
//printf("obtener : %p", abb_obtener(ejemplo, 3) );

  print_test("Elemento pertenece ", abb_pertenece(ejemplo, str) );
  print_test("reemplazar elementos", abb_guardar(ejemplo, str, abb_crear(strcmp, free)));

  //print_test("Elemento pertenece ", abb_pertenece(ejemplo, valores[1]) );
  //print_test("Elemento pertenece ", abb_pertenece(ejemplo, valores[2]) );
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}*/


void pruebas_abb_alumno() {
  prueba_crear_abb_destruir_free();
  prueba_crear_abb_destruir_funcion();
  prueba_crear_abb_destruir_NULL();

  prueba_destruir_abb_vacio_con_free();
  prueba_destruir_abb_vacio_con_funcion();
  prueba_destruir_abb_vacio_con_NULL();

  prueba_abb_esta_vacio();
  prueba_iterar_abb_vacio();

  prueba_destruir_abb_no_vacio_con_free();
  prueba_destruir_abb_no_vacio_con_funcion();
  prueba_abb_borrar_un_elemento();
  prueba_abb_borrar_varios_elementos();
  prueba_iter_interno_sin_corte();
  prueba_iter_interno_con_corte();
  prueba_iter_externo();
  //prueba_pertenece_abb_no_vacio();
}