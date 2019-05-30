#if !defined(__SPILA_H__)
#define __SPILA_H__

// TODO: documentar

#include "../slist/slist.h"
#include "../util.h"

typedef SList SPila;

SPila spila_crear();

int spila_vacia(SPila spila);

void* spila_top(SPila spila);

SPila spila_push(SPila spila, void* dato);

SPila spila_pop(SPila spila, FDestructora destruir_dato);

void spila_destruir(SPila spila, FDestructora destruir_dato);

void spila_recorrer(SPila spila, FVisitante visitar);

void spila_recorrer_extra(SPila spila, FVisitanteExtra visitar, void* extra);

SPila spila_duplicar(SPila spila);

#endif  // __SPILA_H_
