#include "abb.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void abb_destruir_aux(void *abb) { abb_destruir(abb); }

void prueba_crear_abb_destruir_free() {
  printf("------Inicio pruebas crear abb con free\n");
  abb_t *ejemplo = abb_crear(strcmp,free);
  print_test("abb creado", ejemplo != NULL);
  abb_destruir(ejemplo);
}

void prueba_crear_abb_destruir_funcion() {
  printf("------Inicio pruebas crear abb con función destruir\n");
  abb_t *ejemplo = abb_crear(strcmp,abb_destruir_aux);
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

  print_test("abb después de insertar valores NO está vacío", abb_cantidad(ejemplo));
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

  print_test("abb después de insertar valores NO está vacío",abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}


//esta prueba creo que deberiamos cambiarla antes de entregar, pero la dejo mientras porque para probar es útil
bool imprimir(const char* clave, void *dato, void *extra) {
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

bool imprimir_top_3(const char* clave, void *dato, void *extra) {
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

void pruebas_abb_alumno() {
	prueba_crear_abb_destruir_free();
	prueba_crear_abb_destruir_funcion();
	prueba_destruir_abb_vacio_con_free();
	prueba_destruir_abb_vacio_con_funcion();
	prueba_destruir_abb_no_vacio_con_free();
	prueba_destruir_abb_no_vacio_con_funcion();
	prueba_iter_interno_sin_corte();
	prueba_iter_interno_con_corte();
}