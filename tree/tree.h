#if !defined(__TREE_H__)
#define __TREE_H__
#include <stdio.h>
#include <stdlib.h>
#include "../slist/slist.h"

#define TAMANO_ALFABETO 33
int funcion_alfabeto(char caracter);

typedef struct _TNodo {
  struct _TNodo* padre;
  struct _TNodo* hijos[TAMANO_ALFABETO];
  int identificador;
  int termina;
  // int maxProfundidad;
} TNodo;

// tnodo_crear: -> TNodo*
// TODO: completar
TNodo* tnodo_crear();

// tnodo_destruir: TNodo* -> void
// TODO: completar
void tnodo_destruir(TNodo* tnodo);

// tnodo_buscar: TNodo* char* int -> int
// Recibe un puntero a un TNodo, una palabra y un índice,
// TODO: completar
int tnodo_buscar(TNodo* tnodo, char* palabra, int i);

typedef struct {
  TNodo* origen;
} Tree;

// tree_crear: -> Tree
// Crea e inicializa un árbol,
// Lo devuelve.
// TODO: corregir
Tree* tree_crear();

// tree_agregar: Tree char* -> void
// Recibe un árbol y una cadena,
// Agregrega la cadena al árbol
// TODO: corregir
void tree_agregar(Tree* tree, char* palabra);

// tree_contiene: Tree char* -> int
// Recibe un árbol y una palabra,
// Devuelve 1 si el arbol contiene la cadena, 0 en el caso contrario
// TODO: corregir
int tree_contiene(Tree* tree, char* palabra);

// TODO: completar
SList tree_sugerir(Tree* tree, char* palabra);

// tree_destruir: Tree -> void
// Recibe un árbol,
// Destruye el árbol
// TODO: corregir
void tree_destruir(Tree* tree);

#endif  // __TREE_H__
