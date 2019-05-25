#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree/tree.h"

int main(int argc, char* argv[]) {
  // if (argc != 3) {
  //   printf("Error: el número de argumentos ingresados es incorrecto.\n");
  //   printf("Modo de uso: %s [diccionario.txt] [texto.txt]", argv[0]);
  //   return 1;
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
    // if (!tree_contiene(tree, buffer)) {
    // tree_sugerir(tree, buffer);
    // printf("%s\n", (char*)sugerencias->dato);
    // slist_destruir(sugerencias, destruir_generico);
    // }
    scanf("%s", buffer);
  }

  tree_destruir(tree);

  return 0;
}
