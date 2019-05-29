#if !defined(__CDLIST_H__)
#define __CDLIST_H__

// TODO: documentar

#include "../util.h"

typedef struct _CDNodo {
  void* dato;
  struct _CDNodo* ant;
  struct _CDNodo* sig;
} CDNodo;

typedef CDNodo* CDList;

CDList cdlist_crear();

int cdlist_vacia(CDList cdlist);

CDList cdlist_agregar_inicio(CDList cdlist, void* dato);

CDList cdlist_agregar_final(CDList cdlist, void* dato);

CDList cdlist_eliminar_inicio(CDList cdlist, FDestructora destruir_dato);

void cdlist_destruir(CDList cdlist, FDestructora destruir_dato);

void cdlist_recorrer(CDList cdlist, FVisitante visitar);

#endif  // __CDLIST_H__
