#if !defined(__SLIST_H__)
#define __SLIST_H__

// TODO: corregir documentacion

#include "../util.h"

// Un SNodo es un nodo de una lista simplemente enlazada.
typedef struct _SNodo {
  void* dato;
  struct _SNodo* sig;
} SNodo;

// Una SList es una lista simplemente enlazada.
typedef SNodo* SList;

// slist_crear: -> SList
// Devuelve una lista vacía.
SList slist_crear();

// slist_vacia: SList -> int
// Recibe una lista,
// Devuelve 1 si la lista es vacía, 0 en caso contrario.
int slist_vacia(SList slist);

// slist_agregar_inicio: SList void* -> SList
// Recibe una lista y un puntero a un dato,
// La agrega al inicio,
// Devuelve la lista.
SList slist_agregar_inicio(SList slist, void* dato);

// slist_eliminar_inicio: SList -> SList
// Recibe una lista,
// Devuelve la lista sin el primer dato.
SList slist_eliminar_inicio(SList slist, FDestructora destruir_dato);

// slist_destruir: SList -> void
// Recibe una lista,
// La destruye.
void slist_destruir(SList slist, FDestructora destruir_dato);

// TODO: corregir
// slist_imprimir: SList -> void
// Recibe una lista,
// La imprime.
void slist_recorrer(SList slist, FVisitante visitar);

// TODO: documentar
void slist_recorrer_extra(SList slist, FVisitanteExtra visitar, void* extra);

// slist_longitud: SList -> int
// Recibe una lista,
// Devuelve su longitud.
int slist_longitud(SList slist);

// slist_contiene: SList void* -> int
// Recibe una lista y un puntero a un dato,
// Devuelve 1 si la lista contiene a la cadena, 0 en caso contrario.
// int slist_contiene(SList slist, void* dato);

#endif  // __SLIST_H__
