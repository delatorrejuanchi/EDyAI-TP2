#if !defined(__CDCOLA__)
#define __CDCOLA__

#include "../cdlist/cdlist.h"
#include "../util.h"

typedef CDList CDCola;

CDCola cdcola_crear();

int cdcola_vacia(CDCola cdcola);

void* cdcola_primero(CDCola cdcola);

CDCola cdcola_encolar(CDCola cdcola, void* dato);

CDCola cdcola_desencolar(CDCola cdcola, FDestructora destruir_dato);

void cdcola_destruir(CDCola cdcola, FDestructora destruir_dato);

void cdcola_recorrer(CDCola cdcola, FVisitante visitar);

#endif  // __CDCOLA__
