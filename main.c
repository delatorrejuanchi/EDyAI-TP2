#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "tree.h"

int main(int argc, char* argv[]) {
  // setlocale(LC_ALL, "es");

  // char cadena[3];
  // cadena[0] = -61;
  // cadena[2] = '\0';
  // for (int i = -127; i < 0; i++) {
  //   cadena[1] = i;
  //   printf("%s %d\n", cadena, i);
  // }

  Tree* tree = tree_crear();

  char buffer[50];
  scanf("%s", buffer);
  while (strcmp(buffer, ".")) {
    tree_agregar(tree, buffer);
    scanf("%s", buffer);
  }

  scanf("%s", buffer);
  while (strcmp(buffer, ".")) {
    printf("%d\n", tree_contiene(tree, buffer));
    scanf("%s", buffer);
  }

  tree_destruir(tree);

  // if (argc != 3) {
  //   printf("Error: el número de argumentos ingresados es incorrecto.\n");
  //   printf("Modo de uso: %s [diccionario.txt] [texto.txt]", argv[0]);
  //   return 1;
  // }

  // TablaHash* tabla = tablahash_crear(TAMANO_TABLAHASH, hash);
  // TODO: preguntar como determinar un buen tamaño. Se que tiene que ser primo.
  // TODO: mejorar funcion hash

  // int cargada = cargar_tabla(tabla, argv[1]);
  // if (!cargada) {
  //   printf("Error: el archivo %s no existe.\n", argv[1]);
  //   tablahash_destruir(tabla);
  //   return 1;
  // }

  // FILE* out = fopen("out.txt", "w+");
  // for (int i = 0; i < TAMANO_TABLAHASH; i++) {
  //   fprintf(out, "%d %d\n", i, slist_longitud(tabla->tabla[i]));
  // }
  // fclose(out);

  // char palabra[50];
  // while (strcmp(palabra, "exit")) {
  //   scanf("%s", palabra);

  //   if (!tablahash_contiene(tabla, palabra))
  //     generar_sugerencias(tabla, palabra);
  // }

  // char palabra[50];
  // while (strcmp(palabra, "exit") != 0) {
  //   scanf("%s", palabra);

  //   int correcta = tablahash_contiene(tabla, palabra);
  //   if (!correcta) {
  //     printf("\"%s\" no está en el diccionario.\n", palabra);
  //     SList sugerencias = slist_crear();

  //     int longitud = strlen(palabra);

  //     // TODO: no checkear la misma palabra longitud veces xd
  //     // Cambiando una letra
  //     for (int i = 0; i < longitud; i++) {
  //       char original = palabra[i];
  //       for (int c = 'a'; c <= 'z'; c++) {
  //         palabra[i] = c;
  //         if (tablahash_contiene(tabla, palabra)) {
  //           sugerencias = agregar_sugerencia(sugerencias, palabra);
  //         }
  //       }
  //       palabra[i] = original;
  //     }

  //     // TODO: no agrega al inicio
  //     // Agregando una letra
  //     for (int i = longitud; i > 0; i--) {
  //       palabra[i + 1] = palabra[i];
  //     }
  //     for (int i = 1; i < longitud + 1; i++) {
  //       for (int c = 'a'; c <= 'z'; c++) {
  //         palabra[i] = c;
  //         if (tablahash_contiene(tabla, palabra)) {
  //           char* sugerencia = malloc(sizeof(char) * (strlen(palabra) + 1));
  //           strcpy(sugerencia, palabra);

  //           sugerencias = slist_agregar_inicio(sugerencias, sugerencia);
  //         }
  //       }
  //       palabra[i] = palabra[i + 1];
  //     }
  //     palabra[longitud] = '\0';

  //     // Eliminando un caracter
  //     char eliminado = palabra[0];
  //     for (int i = 0; i < longitud; i++) palabra[i] = palabra[i + 1];
  //     for (int i = 0; i < longitud; i++) {
  //       if (tablahash_contiene(tabla, palabra)) {
  //         sugerencias = agregar_sugerencia(sugerencias, palabra);
  //       }

  //       char tmp = eliminado;
  //       eliminado = palabra[i];
  //       palabra[i] = tmp;
  //     }
  //     palabra[strlen(palabra)] = eliminado;
  //     palabra[strlen(palabra) + 1] = '\0';

  //     // Separando la palabra
  //     for (int i = longitud; i > 0; i--) {
  //       palabra[i + 1] = palabra[i];
  //     }
  //     for (int i = 1; i < longitud; i++) {
  //       palabra[i] = '\0';

  //       if (tablahash_contiene(tabla, palabra) &&
  //           tablahash_contiene(tabla, palabra + i + 1)) {
  //         palabra[i] = ' ';
  //         sugerencias = agregar_sugerencia(sugerencias, palabra);
  //       }

  //       palabra[i] = palabra[i + 1];
  //     }
  //     palabra[strlen(palabra) - 1] = '\0';

  //     if (!slist_vacia(sugerencias)) {
  //       printf("Quizás quiso decir: ");
  //       slist_imprimir(sugerencias);
  //     }
  //     slist_destruir(sugerencias);
  //   } else {
  //     printf("Palabra encontrada!\n");
  //   }
  // }

  // tablahash_destruir(tabla);

  return 0;
}
