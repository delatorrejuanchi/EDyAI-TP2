#include "cdlist.h"
#include <stdio.h>
#include <stdlib.h>

CDList cdlist_crear() { return NULL; }

int cdlist_vacia(CDList cdlist) { return cdlist == NULL; }

CDList cdlist_agregar_inicio(CDList cdlist, int dato) {
  CDNodo *nuevoNodo = malloc(sizeof(CDNodo));
  nuevoNodo->dato = dato;

  if (cdlist_vacia(cdlist)) {
    nuevoNodo->sig = nuevoNodo->ant = nuevoNodo;
    return nuevoNodo;
  }

  nuevoNodo->sig = cdlist;
  nuevoNodo->ant = cdlist->ant;

  cdlist->ant = nuevoNodo;
  nuevoNodo->ant->sig = nuevoNodo;

  return nuevoNodo;
}

CDList cdlist_agregar_final(CDList cdlist, int dato) {
  return cdlist_agregar_inicio(cdlist, dato)->sig;
}

CDList cdlist_eliminar_inicio(CDList cdlist) {
  cdlist->ant->sig = cdlist->sig;
  cdlist->sig->ant = cdlist->ant;
  CDList sig = cdlist->sig;
  free(cdlist);

  return sig;
}

void cdlist_destruir(CDList cdlist) {
  CDNodo *nodoAEliminar = cdlist->sig;
  while (nodoAEliminar != cdlist) {
    CDNodo *tmp = nodoAEliminar;
    nodoAEliminar = nodoAEliminar->sig;
    free(tmp);
  }

  free(cdlist);
}

void cdlist_imprimir(CDList cdlist) {
  CDNodo *nodo = cdlist;

  do {
    printf("%d ", nodo->dato);
    nodo = nodo->sig;
  } while (nodo != cdlist);

  printf("\n");
}
