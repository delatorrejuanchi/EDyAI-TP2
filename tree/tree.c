#include "tree.h"
#include <assert.h>
#include <string.h>
#include "../cdcola/cdcola.h"
#include "../spila/spila.h"
#include "../util.h"

// TODO:
// - renombrar: TNodo*->termina
//              Caracter
//              Estructura
typedef struct {
  char caracter;
} Caracter;

typedef struct {
  TNodo* nodo;
  SPila ancestros;
  int i;
} Estructura;

Estructura* estructura_crear(TNodo* nodo, SPila ancestros, int i) {
  Estructura* estructura = malloc(sizeof(Estructura));
  estructura->nodo = nodo;
  estructura->ancestros = ancestros;
  estructura->i = i;

  return estructura;
}

Caracter* caracter_crear(char c) {
  Caracter* caracter = malloc(sizeof(Caracter));
  caracter->caracter = c;

  return caracter;
}

TNodo* tnodo_crear() {
  TNodo* tnodo = malloc(sizeof(TNodo));
  tnodo->padre = NULL;
  tnodo->termina = 0;
  tnodo->identificador = -1;
  // tnodo->maxProfundidad = 0;

  for (int i = 0; i < TAMANO_ALFABETO; i++) {
    tnodo->hijos[i] = NULL;
  }

  return tnodo;
}

int tnodo_buscar(TNodo* nodo, char* palabra, int i) {
  for (int j = i; j < strlen(palabra) && nodo != NULL; j++) {
    int indice = caracter_a_indice(palabra[j]);
    if (indice != -1) nodo = nodo->hijos[indice];
  }

  return nodo != NULL && nodo->termina;
}

void tnodo_destruir(TNodo* tnodo) {
  if (tnodo != NULL) {
    for (int i = 0; i < TAMANO_ALFABETO; i++) {
      tnodo_destruir(tnodo->hijos[i]);
    }

    free(tnodo);
  }
}

Tree* tree_crear() {
  Tree* tree = malloc(sizeof(TNodo));
  tree->origen = tnodo_crear();

  return tree;
}

void tree_destruir(Tree* tree) {
  tnodo_destruir(tree->origen);
  free(tree);
}

int tree_contiene(Tree* tree, char* palabra) {
  return tnodo_buscar(tree->origen, palabra, 0);
}

void tree_agregar(Tree* tree, char* palabra) {
  TNodo* nodo = tree->origen;
  for (int i = 0; i < strlen(palabra); i++) {
    int indice = caracter_a_indice(palabra[i]);
    if (indice != -1) {
      if (nodo->hijos[indice] == NULL) {
        nodo->hijos[indice] = tnodo_crear();
        nodo->hijos[indice]->padre = nodo;
        nodo->hijos[indice]->identificador = indice;
      }

      nodo = nodo->hijos[indice];
    }
  }

  nodo->termina = 1;
}

// TODO: Renombrar
void caracteres_agregar(TNodo* nodo, SPila* caracteres) {
  TNodo* nodoActual = nodo;
  while (nodoActual->padre != NULL) {
    int indice = nodoActual->identificador;
    Caracter* caracter = caracter_crear(indice_a_caracter(indice));
    *caracteres = spila_push(*caracteres, caracter);
    if (indice >= 26) {
      Caracter* especial = caracter_crear(-61);
      *caracteres = spila_push(*caracteres, especial);
    }
    nodoActual = nodoActual->padre;
  }
}

char* resconstruir(SPila ancestros, TNodo* nodo, char* palabra, int i) {
  if (nodo == NULL)
    printf("WTF?!\n");
  else
    printf("%d-i:%d %d\n", nodo->identificador, i,
           nodo->padre->padre->identificador);
  SPila caracteres = spila_crear();

  caracteres_agregar(nodo, &caracteres);

  SNodo* ancestro = ancestros;
  TNodo* nodoActual;
  while (ancestro != NULL) {
    nodoActual = ancestro->dato;
    Caracter* espacio = caracter_crear(' ');
    caracteres = spila_push(caracteres, espacio);
    caracteres_agregar(nodoActual, &caracteres);
    ancestro = ancestro->sig;
  }

  int prefijo = slist_longitud(caracteres), sufijo = strlen(palabra) - i;
  char* sugerencia = malloc(sizeof(char) * (prefijo + sufijo + 1));

  int c = 0;
  while (!spila_vacia(caracteres)) {
    sugerencia[c] = ((Caracter*)spila_top(caracteres))->caracter;
    caracteres = spila_pop(caracteres, destruir_generico);
    c++;
  }

  for (int j = i; j < strlen(palabra); j++) {
    sugerencia[c] = palabra[j];
    c++;
  }

  sugerencia[c] = '\0';

  return sugerencia;
}

void imprimir_estructura(void* dato) {
  Estructura* estructura = dato;
  SPila caracteres = spila_crear();
  if (estructura->nodo == NULL) {
    printf("END\n");
    return;
  }

  caracteres_agregar(estructura->nodo, &caracteres);

  // SNodo* ancestro = estructura->ancestros;
  // TNodo* nodoActual;
  // while (ancestro != NULL) {
  //   nodoActual = ancestro->dato;
  //   Caracter* espacio = caracter_crear(' ');
  //   caracteres = spila_push(caracteres, espacio);
  //   caracteres_agregar(nodoActual, &caracteres);
  //   ancestro = ancestro->sig;
  // }

  char palabra[100];

  int c = 0;
  while (!spila_vacia(caracteres)) {
    palabra[c] = ((Caracter*)spila_top(caracteres))->caracter;
    caracteres = spila_pop(caracteres, destruir_generico);
    c++;
  }

  palabra[c] = '\0';

  printf("--> %s\n", palabra);
}

SList tree_sugerir(Tree* tree, char* palabra) {
  int longitud = strlen(palabra);

  // Inicializamos una lista de sugerencias
  SList sugerencias = slist_crear();

  // Inicializamos una cola de "estructura" jejexd TODO: renombrar
  CDCola cola = cdcola_crear();
  cola = cdcola_encolar(cola, estructura_crear(tree->origen, spila_crear(), 0));
  // cola = cdcola_desencolar(cola, destruir_generico);
  // cola = cdcola_desencolar(cola, destruir_generico);

  while (slist_longitud(sugerencias) <= 2 && !cdcola_vacia(cola)) {
    Estructura* estructura = cdcola_primero(cola);
    TNodo* nodoBase = estructura->nodo;
    SPila ancestros = estructura->ancestros;
    int i = estructura->i;
    cola = cdcola_desencolar(cola, destruir_generico);

    // Agregar letra
    TNodo* nodo = nodoBase;
    for (int j = 0; j <= longitud - i && nodo != NULL; j++) {
      int indice = caracter_a_indice(palabra[j + i]);
      if (indice != -1 || j == longitud - i) {
        for (int c = 0; c < TAMANO_ALFABETO; c++) {
          if (tnodo_buscar(nodo->hijos[c], palabra, i + j)) {
            char* sugerencia =
                resconstruir(ancestros, nodo->hijos[c], palabra, j);
            printf("%s\n", sugerencia);
            sugerencias = slist_agregar_inicio(sugerencias, sugerencia);
          }

          if (nodo != NULL) {
            estructura = estructura_crear(nodo->hijos[c], ancestros, i + j);
            cola = cdcola_encolar(cola, estructura);
          }
        }
        nodo = nodo->hijos[indice];
      }
    }

    // cdcola_recorrer(cola, imprimir_estructura);

    // Eliminar letra
    // nodoActual = nodo;
    // for (int nivel = 0; nivel < longitud - i; nivel++) {
    //   int indice = caracter_a_indice(palabra[nivel + i + 1]);

    //   if (tnodo_buscar(nodoActual->hijos[indice], palabra, nivel + i + 1)) {
    //     // agregar a sugerencias
    //   }

    //   cola = cdcola_encolar(cola,
    //                         2);  // struct nodo=nodo->hijos[indice],
    //   i = nivel + i +
    //       1

    //       indice = caracter_a_indice(palabra[i + nivel]);
    //   nodoActual = nodoActual->hijos[indice];
    // }

    // // Cambiar letra
    // TNodo* nodoActual = nodo;
    // for (int nivel = 0; nivel < longitud(palabra) - i; nivel++) {
    //   for (int c = 0; c < TAMANO_ALFABETO; c++) {
    //     if (tnodo_buscar(nodoActual->hijos[c], palabra, nivel + i + 1)) {
    //       // agregar a sugerencias
    //     }

    //     cola = cdcola_encolar(cola, 1);
    //     // struct nodo=nodo->hijos[c], i=nivel+i+1
    //   }

    //   int indice = caracter_a_indice(palabra[i + nivel]);
    //   nodoActual = nodoActual->hijos[indice];
    // }

    // // Agrregar espacios
    // TNodo* nodoActual = nodo;
    // for (int nivel = 0; nivel <= longitud(palabra) - i; nivel++) {
    //   if (nodoActual->termina && tnodo_buscar(tree->origen, palabra, i + 1))
    //   {
    //     // agregar a sugerencias
    //   }

    //   cola = cdcola_encolar(cola, 1);  // struct nodo=tree->origen,
    //   i = nivel + i + 1,
    //   // padres=padres+nodoActual

    //       int indice = caracter_a_indice(palabra[i + nivel]);
    //   nodoActual = nodoActual->hijos[indice];
    // }
  }

  return sugerencias;
}
