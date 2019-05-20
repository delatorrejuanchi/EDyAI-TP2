#if !defined(__SLIST__)
#define __SLIST__

// Un SNodo es un nodo de una lista simplemente enlazada de cadenas.
typedef struct _SNodo {
  char* dato;
  struct _SNodo* sig;
} SNodo;

// Una SList es una lista simplemente enlazada de cadenas.
typedef SNodo* SList;

// slist_crear: -> SList
// Devuelve una lista vacía.
SList slist_crear();

// slist_vacia: SList -> int
// Recibe una lista,
// Devuelve 1 si la lista es vacía, 0 en caso contrario.
int slist_vacia(SList slist);

// slist_agregar_inicio: SList char* -> SList
// Recibe una lista y una cadena,
// La agrega al inicio,
// Devuelve la lista.
SList slist_agregar_inicio(SList slist, char* dato);

// slist_eliminar_inicio: SList -> SList
// Recibe una lista,
// Elimina el primer dato y devuelve la lista.
SList slist_eliminar_inicio(SList slist);

// slist_destruir: SList -> void
// Recibe una lista,
// La destruye.
void slist_destruir(SList slist);

// slist_imprimir: SList -> void
// Recibe una lista,
// La imprime.
void slist_imprimir(SList slist);

// slist_longitud: SList -> int
// Recibe una lista,
// Devuelve su longitud
int slist_longitud(SList slist);

// slist_contiene: SList char* -> int
// Recibe una lista y una cadena,
// Devuelve 1 si la lista contiene a la cadena, 0 en caso contrario.
int slist_contiene(SList slist, char* dato);

#endif  // __SLIST__
