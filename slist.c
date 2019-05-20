#include "slist.h"
#include <assert.h>
#include <stdlib.h>

SList slist_crear() { return NULL; }

int slist_vacia(SList slist) { return slist == NULL; }

SList slist_agregar_inicio(SList slist, char* dato) {
  SNodo* nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = slist;

  return nuevoNodo;
}

SList slist_eliminar_inicio(SList slist) {
  assert(!slist_vacia(slist));

  SNodo* nodoAEliminar = slist;
  slist = slist->sig;

  free(nodoAEliminar->dato);
  free(nodoAEliminar);

  return slist;
}

void slist_destruir(SList slist) {
  while (!slist_vacia(slist)) {
    slist = slist_eliminar_inicio(slist);
  }
}

void slist_imprimir(SList slist) {
  for (SNodo* nodo = slist; nodo != NULL; nodo = nodo->sig) {
    printf("%s ", nodo->dato);
  }

  printf("\n");
}

int slist_longitud(SList slist) {
  int longitud = 0;
  for (SNodo* nodo = slist; nodo != NULL; nodo = nodo->sig) longitud++;

  return longitud;
}

int slist_contiene(SList slist, char* dato) {
  SNodo* nodo = slist;
  while (nodo != NULL && strcmp(nodo->dato, dato) != 0) nodo = nodo->sig;

  return nodo != NULL;
}
