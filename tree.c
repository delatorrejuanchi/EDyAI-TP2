#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define STOP 0

Tree tree_crear() {
  Tree tree = malloc(sizeof(Tree));
  tree->funcion = caracter_a_indice;
  tree->hijos = NULL;
}

int tree_es_vacio(Tree tree) {
  return tree->hijos == NULL;
}

// TODO: Que es mejor assert o if si hay que ver si esta vacia
// TODO: Ver si esta bien
void tree_destruir(Tree tree) {
  assert(!tree_es_vacio(tree));
  tree_destruir(tree->hijos);
  free(tree);
}

int tree_contiene_cadena(Tree tree, char* cadena) {
  if (!tree_es_vacio(tree)) {
    int longitud = strlen(cadena), found = 0;
    TNodo *nodo = tree->hijos;

    for (int i = 0; i < longitud && nodo != NULL  && !found; i++) {
      int letra = tree->funcion(cadena[i]);

      if (!nodo->letras[letra]->termina) nodo = nodo->letras[letra];

      else found = 1;
    }

    return found;
  }
}

// TODO: Recorro 2 veces el tree
void tree_insertar(Tree tree, char* cadena) {
    TNodo *nodo = tree->hijos;
    int longitud = 0;

    for (int i = 0; i < longitud; i++) {
      int letra = tree->funcion(cadena[i]);
      nodo->termina = 0;
      nodo = nodo->letras[letra];
    }

    nodo->termina = 1;
}