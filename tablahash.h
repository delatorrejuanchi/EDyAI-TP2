#if !defined(__TABLAHASH_H__)
#define __TABLAHASH_H__
#include <stdlib.h>
#include "slist.h"

#define TAMANO_TABLAHASH 1499

// Una FuncionHash recibe una cadena y devuelve un entero positivo.
// Este tipo de funcion es pasado como argumento en la funcion tablahash_crear
typedef unsigned int (*FuncionHash)(char *dato);

// TODO: Comentar
typedef struct {
  SList *tabla;
  unsigned int nElems;
  unsigned int capacidad;
  FuncionHash hash;
} TablaHash;

// tablahash_crear: unsigned int FuncionHash -> TablaHash*
// Recibe un entero positivo y una funcion hash,
// Crea e inicializa una tabla hash,
// Devuelve una tabla hash
TablaHash *tablahash_crear(unsigned int capacidad, FuncionHash hash);

// tablahash_insertar: TablaHash* char* -> void
// Recibe una tabla hash y dato,
// Inserta el dato en la tabla hash,
void tablahash_insertar(TablaHash *tabla, char *dato);

// tablahash_contiene: TablaHash* char* -> int
// Recibe una tabla hash y un dato,
// Devuelve 1 si el dato esta en la tabla hash o 0 en el caso contrario
int tablahash_contiene(TablaHash *tabla, char *dato);

// tablahash_destruir: TablaHash -> void
// Recibe una tabla hash
// Destruye la tabla hash
void tablahash_destruir(TablaHash *tabla);

#endif  // __TABLAHASH_H__
