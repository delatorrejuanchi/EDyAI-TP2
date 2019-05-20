#include "tablahash.h"

TablaHash *tablahash_crear(unsigned int capacidad, FuncionHash hash) {
  TablaHash *tabla = malloc(sizeof(TablaHash));
  tabla->tabla = malloc(sizeof(SList) * capacidad);
  for (int i = 0; i < capacidad; i++) {
    tabla->tabla[i] = slist_crear();
  }

  tabla->capacidad = capacidad;
  tabla->nElems = 0;
  tabla->hash = hash;

  return tabla;
}

void tablahash_insertar(TablaHash *tabla, char *dato) {
  unsigned int id = tabla->hash(dato) % tabla->capacidad;
  tabla->tabla[id] = slist_agregar_inicio(tabla->tabla[id], dato);
  tabla->nElems++;
}

int tablahash_contiene(TablaHash *tabla, char *dato) {
  unsigned id = tabla->hash(dato) % tabla->capacidad;

  return slist_contiene(tabla->tabla[id], dato);
}

void tablahash_destruir(TablaHash *tabla) {
  for (int i = 0; i < tabla->capacidad; i++) {
    slist_destruir(tabla->tabla[i]);
  }
  free(tabla->tabla);
  free(tabla);
}