#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "trie/trie.h"

// Representa la cantidad de sugerencias a generar por cada
// palabra incorrecta encontrada.
#define CANTIDAD_SUGERENCIAS 5

// cargar_diccionario: Trie* char* -> int
// Recibe un puntero a un Trie y un el nombre de archivo de un diccionario,
// Si el archivo existe, agrega todas las palabras al Trie, y devuelve 1.
// Si no, devuelve 0.
// TODO: ver cuanto mejora si hacemos un read mayor
int cargar_diccionario(Trie* trie, char* nombreDeArchivo) {
  FILE* archivo = fopen(nombreDeArchivo, "r");

  if (archivo == NULL) return 0;

  char buffer[100];
  while (fscanf(archivo, "%s", buffer) != EOF) trie_agregar(trie, buffer);

  fclose(archivo);

  return 1;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Error: el número de argumentos ingresados es incorrecto.\n");
    printf("Modo de uso: %s [diccionario.txt] [texto.txt]\n", argv[0]);
    return 1;
  }

  Trie* trie = trie_crear();
  char b[50];
  printf("Activar modo debug? ");
  scanf("%s", b);
  if (strcmp(b, "si") != 0) {
    clock_t t;
    t = clock();
    if (cargar_diccionario(trie, argv[1])) {
      printf("Diccionario %s cargado exitosamente.\n", argv[1]);
    } else {
      printf("Se ha producido un error cargando el diccionario %s\n.", argv[1]);
    }
    t = clock() - t;

    printf("CARGA DE DICCIONARIO: %lfs\n", ((double)t) / CLOCKS_PER_SEC);
  } else {
    printf("Inserte el diccionario:\n");
    scanf("%s", b);
    while (strcmp(b, ".")) {
      trie_agregar(trie, b);
      scanf("%s", b);
    }
  }

  char buffer[50];
  printf("Buscar palabra: ");
  scanf("%s", buffer);
  while (strcmp(buffer, ".")) {
    if (trie_contiene(trie, buffer)) {
      printf("Palabra encontrada!\n");
    } else {
      printf("\"%s\" no está en el diccionario.\n", buffer);
      clock_t t = clock();
      Arreglo* sugerencias = trie_sugerir(trie, buffer, CANTIDAD_SUGERENCIAS);
      t = clock() - t;
      printf("Quizás quiso decir:");
      for (int i = 0; i < CANTIDAD_SUGERENCIAS; i++)
        printf(" \"%s\"", sugerencias->datos[i]);
      printf("\n");

      printf("CARGA DE SUGERENCIAS: %lfs\n", ((double)t) / CLOCKS_PER_SEC);

      arreglo_destruir(sugerencias);
    }

    printf("Buscar palabra: ");
    scanf("%s", buffer);
  };

  trie_destruir(trie);

  return 0;
}
