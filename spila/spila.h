#ifndef __SPILA__
#define __SPILA__

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

#endif /* __SPILA__ */
