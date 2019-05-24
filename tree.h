#if !defined(__TREE_H__)
#define __TREE_H__
#include <stdio.h>
#include <stdlib.h>

typedef int (*FuncionX) (char caracter);
// La letra a = 97 en la tabla ASCII
int caracter_a_indice(char caracter) {
  int indice;
  if ('a' <= caracter || caracter <= 'z') indice = caracter - 97;
  return indice;
}

typedef struct _TNodo {
  TNodo* letras[33];
  int termina;
} TNodo;

typedef struct _Tree{
  TNodo *hijos;
  FuncionX funcion;
} *Tree;

// tree_crear:  -> Tree
// Crea e inicializa un árbol,
// Devuelve un árbol
Tree tree_crear();

// tree_es_vacio: Tree -> int
// Reibe un árbol,
// Devuelve 1 si el árbol esta vacio, 0 en el caso contrario
int tree_es_vacio(Tree tree);

// tree_destruir: Tree -> void
// Recibe un árbol,
// Destruye el árbol
void tree_destruir(Tree tree);

// tree_contiene_cadena: Tree char* -> int
// Recibe un árbol y una cadena,
// Devuelve 1 si el arbol contiene la cadena, 0 en el caso contrario
int tree_contiene_cadena(Tree tree, char* cadena);

// tree_insertar: Tree char* -> void
// Recibe un árbol y una cadena,
// Agregrega la cadena al árbol
void tree_insertar(Tree tree, char* cadena);

#endif  // __TREE_H__
