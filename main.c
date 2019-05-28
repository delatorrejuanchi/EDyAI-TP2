#include <stdio.h>
#include <string.h>
#include "tree/tree.h"

// TODO: tratar de reducir encolamientos redundantes

int cargar_diccionario(Tree* tree, char* nombreDeArchivo) {
  FILE* archivo = fopen(nombreDeArchivo, "r");

  if (archivo == NULL) return 0;

  char buffer[100];
  while (fscanf(archivo, "%s", buffer) != EOF) {
    char* palabra = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(palabra, buffer);

    tree_agregar(tree, palabra);
  }

  fclose(archivo);

  return 1;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Error: el número de argumentos ingresados es incorrecto.\n");
    printf("Modo de uso: %s [diccionario.txt] [texto.txt]\n", argv[0]);
    return 1;
  }

  Tree* tree = tree_crear();
  if (cargar_diccionario(tree, argv[1])) {
    printf("Diccionario %s cargado exitosamente.\n", argv[1]);
  } else {
    printf("Se ha producido un error al cargar el diccionario %s\n.", argv[1]);
  }

  // char b[50];
  // scanf("%s", b);
  // while (strcmp(b, ".")) {
  //   // printf("%d\n", tree_contiene(tree, buffer));
  //   char* palabra = malloc(sizeof(char) * (strlen(b) + 1));
  //   strcpy(palabra, b);
  //   tree_agregar(tree, palabra);
  //   scanf("%s", b);
  // }

  char buffer[50];
  do {
    printf("Buscar palabra:> ");
    scanf("%s", buffer);
    // printf("%d\n", tree_contiene(tree, buffer));
    if (tree_contiene(tree, buffer)) {
      printf("Palabra encontrada!\n");
    } else {
      printf("Quizás quiso decir:\n");
      SList sugerencias = tree_sugerir(tree, buffer);
      printf("%d\n", slist_longitud(sugerencias));
      slist_destruir(sugerencias, destruir_generico);
    }
  } while (strcmp(buffer, "."));

  tree_destruir(tree);

  return 0;
}
