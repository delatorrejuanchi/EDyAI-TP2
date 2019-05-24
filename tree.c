#include "tree.h"
#include <assert.h>
#include <string.h>

int funcion_alfabeto(char caracter) {
  int indice = -1;
  if ('a' <= caracter && caracter <= 'z')
    indice = caracter - 97;
  else {
    switch ((int)caracter) {
      case -95:
        indice = 26;
        break;
      case -87:
        indice = 27;
        break;
      case -83:
        indice = 28;
        break;
      case -77:
        indice = 29;
        break;
      case -70:
        indice = 30;
        break;
      case -68:
        indice = 31;
        break;
      case -79:
        indice = 32;
        break;
    }
  }
  return indice;
}

Tree* tree_crear() {
  Tree* tree = malloc(sizeof(TNodo));
  tree->origen = tnodo_crear();

  return tree;
}

// int tree_vacio(Tree* tree) { return tree->origen == NULL; }

TNodo* tnodo_crear() {
  TNodo* tnodo = malloc(sizeof(TNodo));
  tnodo->termina = 0;
  // tnodo->maxProfundidad = 0;
  for (int i = 0; i < TAMANO_ALFABETO; i++) {
    tnodo->hijos[i] = NULL;
  }

  return tnodo;
}

void tree_destruir(Tree* tree) {
  tnodo_destruir(tree->origen);
  free(tree);
}

void tnodo_destruir(TNodo* tnodo) {
  if (tnodo != NULL) {
    for (int i = 0; i < TAMANO_ALFABETO; i++) {
      tnodo_destruir(tnodo->hijos[i]);
    }

    free(tnodo);
  }
}

int tree_contiene(Tree* tree, char* palabra) {
  int longitud = strlen(palabra);

  TNodo* nodo = tree->origen;
  for (int i = 0; i < longitud && nodo != NULL; i++) {
    int indice = funcion_alfabeto(palabra[i]);
    if (indice != -1) nodo = nodo->hijos[indice];
  }

  return nodo != NULL && nodo->termina;
}

void tree_agregar(Tree* tree, char* palabra) {
  int longitud = strlen(palabra);

  TNodo* nodo = tree->origen;
  for (int i = 0; i < longitud; i++) {
    int indice = funcion_alfabeto(palabra[i]);
    if (indice != -1) {
      if (nodo->hijos[indice] == NULL) {
        nodo->hijos[indice] = tnodo_crear();
      }

      nodo = nodo->hijos[indice];
    }
  }

  nodo->termina = 1;
}
