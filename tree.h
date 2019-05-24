#if !defined(__TREE_H__)
#define __TREE_H__
#include <stdio.h>
#include <stdlib.h>

#define TAMANO_ALFABETO 33

typedef struct _TNodo {
  struct _TNodo* hijos[TAMANO_ALFABETO];
  int termina;
  // int maxProfundidad;
} TNodo;

typedef struct {
  TNodo* origen;
} Tree;

TNodo* tnodo_crear();

void tnodo_destruir(TNodo* tnodo);

// tree_crear:  -> Tree
// Crea e inicializa un árbol,
// Devuelve un árbol
Tree* tree_crear();

// tree_destruir: Tree -> void
// Recibe un árbol,
// Destruye el árbol
void tree_destruir(Tree* tree);

// tree_contiene: Tree char* -> int
// Recibe un árbol y una palabra,
// Devuelve 1 si el arbol contiene la cadena, 0 en el caso contrario
int tree_contiene(Tree* tree, char* palabra);

// tree_agregar: Tree char* -> void
// Recibe un árbol y una cadena,
// Agregrega la cadena al árbol
void tree_agregar(Tree* tree, char* palabra);

#endif  // __TREE_H__
