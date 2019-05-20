#include "slist.h"
#include <stdio.h>
#include <stdlib.h>

SList slist_crear() { return NULL; }

int slist_vacia(SList slist) { return slist == NULL; }

SList slist_agregar_inicio(SList slist, int dato) {
  SNodo* nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = slist;

  return nuevoNodo;
}

SList slist_eliminar_inicio(SList slist) {
  SNodo* nodoAEliminar = slist;
  slist = slist->sig;
  free(nodoAEliminar);
  return slist;
}

void slist_destruir(SList slist) {
  while (slist != NULL) {
    slist = slist_eliminar_inicio(slist);
  }
}

void slist_imprimir(SList slist) {
  for (SNodo* nodo = slist; nodo != NULL; nodo = nodo->sig) {
    printf("%d ", nodo->dato);
  }

  printf("\n");
}
