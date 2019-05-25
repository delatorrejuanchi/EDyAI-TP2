#include "slist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SList slist_crear() { return NULL; }

int slist_vacia(SList slist) { return slist == NULL; }

SList slist_agregar_inicio(SList slist, void* dato) {
  SNodo* nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = slist;

  return nuevoNodo;
}

SList slist_eliminar_inicio(SList slist, FDestructora destruir_dato) {
  assert(!slist_vacia(slist));

  SNodo* nodoAEliminar = slist;
  slist = slist->sig;

  destruir_dato(slist->dato);
  free(nodoAEliminar);

  return slist;
}

void slist_destruir(SList slist, FDestructora destruir_dato) {
  while (!slist_vacia(slist)) {
    slist = slist_eliminar_inicio(slist, destruir_dato);
  }
}

void slist_recorrer(SList slist, FVisitante visitar) {
  for (SNodo* nodo = slist; nodo != NULL; nodo = nodo->sig) {
    visitar(nodo);
  }
}

int slist_longitud(SList slist) {
  int longitud = 0;
  for (SNodo* nodo = slist; nodo != NULL; nodo = nodo->sig) longitud++;

  return longitud;
}

// int slist_contiene(SList slist, char* dato) {
//   SNodo* nodo = slist;
//   while (nodo != NULL && strcmp(nodo->dato, dato) != 0) nodo = nodo->sig;

//   return nodo != NULL;
// }
