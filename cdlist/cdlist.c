#include "cdlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

CDList cdlist_crear() { return NULL; }

int cdlist_vacia(CDList cdlist) { return cdlist == NULL; }

CDList cdlist_agregar_inicio(CDList cdlist, void *dato) {
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

CDList cdlist_agregar_final(CDList cdlist, void *dato) {
  return cdlist_agregar_inicio(cdlist, dato)->sig;
}

CDList cdlist_eliminar_inicio(CDList cdlist, FDestructora destruir_dato) {
  assert(!cdlist_vacia(cdlist));

  CDList sig;
  if (cdlist != cdlist->sig) {
    cdlist->ant->sig = cdlist->sig;
    cdlist->sig->ant = cdlist->ant;
    sig = cdlist->sig;
  } else {
    sig = NULL;
  }

  destruir_dato(cdlist->dato);
  free(cdlist);

  return sig;
}

void cdlist_destruir(CDList cdlist, FDestructora destruir_dato) {
  if (!cdlist_vacia(cdlist)) {
    CDNodo *nodoAEliminar = cdlist->sig;
    while (nodoAEliminar != cdlist) {
      CDNodo *tmp = nodoAEliminar;
      nodoAEliminar = nodoAEliminar->sig;
      destruir_dato(tmp->dato);
      free(tmp);
    }

    destruir_dato(cdlist->dato);
    free(cdlist);
  }
}

void cdlist_recorrer(CDList cdlist, FVisitante visitar) {
  if (!cdlist_vacia(cdlist)) {
    CDNodo *nodo = cdlist;

    do {
      visitar(nodo->dato);
      nodo = nodo->sig;
    } while (nodo != cdlist);
  }
}
