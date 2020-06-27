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
  printf("------Inicio pruebas crear abb con funcion destruir\n");
  abb_t *ejemplo = abb_crear(strcmp,abb_destruir_aux);
  print_test("abb creado", ejemplo != NULL);
  abb_destruir(ejemplo);
}

void prueba_destruir_abb_vacio_con_free() {
  printf("------Inicio pruebas destruir abb vacio con funcion destruir free\n");
  abb_t *ejemplo = abb_crear(strcmp, free);
  print_test("Abb tiene cantidad 0", !abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}

void prueba_destruir_abb_vacio_con_funcion() {
  printf("------Inicio pruebas destruir abb vacio con funcion destruir funcion\n");
  abb_t *ejemplo = abb_crear(strcmp, abb_destruir_aux);
  print_test("Abb tiene cantidad 0", !abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}

void prueba_destruir_abb_no_vacio_con_free() {
  printf("------Inicio pruebas destruir abb no vacio con parámetro free\n");
  abb_t *ejemplo = abb_crear(strcmp, free);
  void *valores[10];
  char str[2];
  
  for (int i = 0; i < 10; i++) {
    valores[i] = malloc(sizeof(char));
    sprintf(str, "%d", i);
    abb_guardar(ejemplo, str, valores[i]);
  }

  printf("Cantidad %d\n", (int)abb_cantidad(ejemplo));
  print_test("abb después de insertar valores NO está vacio", abb_cantidad(ejemplo));
  abb_destruir(ejemplo);
  print_test("abb destruido", true);
}
/*
void prueba_destruir_abb_no_vacia_con_funcion() {
  printf(
      "------Inicio pruebas destruir abb no vacía con parámetro función\n");
  abb_t *ejemplo = abb_crear();
  void *valores[10];

  for (int i = 0; i < 10; i++) {
    valores[i] = abb_crear();
    abb_insertar_primero(ejemplo, valores[i]);
  }

  print_test("abb después de insertar valores NO está vacia",
             !abb_esta_vacia(ejemplo));
  abb_destruir(ejemplo, abb_destruir_aux);
  print_test("abb destruida con funcion", true);
}
*/
void pruebas_abb_alumno() {

	prueba_crear_abb_destruir_free();
	prueba_crear_abb_destruir_funcion();
	prueba_destruir_abb_vacio_con_free();
	prueba_destruir_abb_vacio_con_funcion();
	prueba_destruir_abb_no_vacio_con_free();

}