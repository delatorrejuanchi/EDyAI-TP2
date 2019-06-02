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
  FILE* diccionario = fopen(nombreDeArchivo, "r");

  if (diccionario == NULL) return 0;

  char buffer[50];

  while (fscanf(diccionario, "%s", buffer) != EOF) trie_agregar(trie, buffer);

  fclose(diccionario);

  return 1;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Error: el número de argumentos ingresados es incorrecto.\n");
    printf("Modo de uso: %s [diccionario.txt] [texto.txt] [salida.txt]\n",
           argv[0]);
    return 1;
  }

  Trie* trie = trie_crear();

  clock_t timer;
  timer = clock();
  if (cargar_diccionario(trie, argv[1])) {
    printf("Diccionario %s cargado exitosamente.\n", argv[1]);
  } else {
    printf("Se ha producido un error cargando el diccionario %s.\n", argv[1]);
  }
  timer = clock() - timer;
  printf("CARGA DE DICCIONARIO: %lfs\n", ((double)timer) / CLOCKS_PER_SEC);

  FILE* texto = fopen(argv[2], "r");
  if (texto == NULL) {
    printf("Se ha producido un error leyendo el texto %s.\n", argv[2]);
    return 1;
  }

  FILE* salida = fopen(argv[3], "w");
  if (salida == NULL) {
    printf("Se ha producido un error creando el archivo de salida %s.\n",
           argv[3]);
    return 1;
  }

  timer = clock();
  char buffer[50], c;
  int linea = 1, i = 0, insertandoCaracterEspecial = 0;
  while ((c = fgetc(texto)) != EOF) {
    if (c == -61)
      insertandoCaracterEspecial = 1;
    else {
      if (caracter_a_indice(c, insertandoCaracterEspecial) != -1) {
        if (insertandoCaracterEspecial) {
          buffer[i++] = -61;
          insertandoCaracterEspecial = 0;
        }
        buffer[i++] = c;
      } else if (i != 0) {
        buffer[i] = '\0';
        if (!trie_contiene(trie, buffer)) {
          Arreglo* sugerencias =
              trie_sugerir(trie, buffer, CANTIDAD_SUGERENCIAS);
          fprintf(salida, "Línea %d, \"%s\" no está en el diccionario.\n",
                  linea, buffer);
          fprintf(salida, "Quizás quiso decir: %s", sugerencias->datos[0]);
          for (int i = 1; i < CANTIDAD_SUGERENCIAS; i++) {
            fprintf(salida, ", %s", sugerencias->datos[i]);
          }
          fprintf(salida, "\n\n");
          arreglo_destruir(sugerencias);
        }

        i = 0;
        if (c == '\n') linea++;
      }
    }
  }
  timer = clock() - timer;
  printf("CORRECCIÓN DEL TEXTO: %lfs\n", ((double)timer) / CLOCKS_PER_SEC);

  fclose(texto);
  fclose(salida);

  trie_destruir(trie);

  return 0;
}
