#include "cdcola.h"
#include <assert.h>

CDCola cdcola_crear() { return cdlist_crear(); }

int cdcola_vacia(CDCola cdcola) { return cdlist_vacia(cdcola); }

void* cdcola_primero(CDCola cdcola) {
  assert(!cdcola_vacia(cdcola));

  return cdcola->dato;
}

CDCola cdcola_encolar(CDCola cdcola, void* dato) {
  return cdlist_agregar_final(cdcola, dato);
}

CDCola cdcola_desencolar(CDCola cdcola, FDestructora destruir_dato) {
  return cdlist_eliminar_inicio(cdcola, destruir_dato);
}

void cdcola_destruir(CDCola cdcola, FDestructora destruir_dato) {
  cdlist_destruir(cdcola, destruir_dato);
}

void cdcola_recorrer(CDCola cdcola, FVisitante visitar) {
  cdlist_recorrer(cdcola, visitar);
}
